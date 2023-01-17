/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

  This program can be distributed under the terms of the GNU GPLv2.
  See the file COPYING.
*/

/** @file
 *
 * minimal example filesystem using high-level API
 *
 * Compile with:
 *
 *     gcc -Wall hello.c `pkg-config fuse3 --cflags --libs` -o hello
 *
 * ## Source code ##
 * \include hello.c
 */


#define FUSE_USE_VERSION 31
#define MAX_NODES 10
#define MAX_CHILDREN 5

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>
#include <syslog.h>

/*
 * Command line options
 *
 * We can't set default values for the char* fields here because
 * fuse_opt_parse would attempt to free() them when the user specifies
 * different values on the command line.
 */
static struct options {
	const char *filename;
	const char *contents;
	int show_help;
} options,options_list[5];

static struct node {
	const char *filename;//file or directory name
	const char *contents;//file contents
	const char *path;
	int type; // 0 for file,1 for directory
	mode_t mode;//The type and permissions of the file or directory
    uid_t uid;
	gid_t gid;
	struct node *parent; // parent node
    struct node **children; // children nodes
    int child_count; // numbers of children nodes
	time_t atime;//access time
	time_t mtime;//modification time
	time_t ctime;//state change time
} ;

struct node* nodes[MAX_NODES];//list of node
int file_num=0;//numbers of files and directory

const char * GetFileNamefromPath(const char *path){
	fprintf(stderr, "GetFileNamefromPath is called\n");
	char *filename_temp = strrchr(path, '/');
	printf("filename: %s\n", filename_temp);
	//char *filename=filename_temp+1;
	return strdup(filename_temp + 1);
	//return filename;
}
struct node* CreateNode(const char *path,int type,struct Node *parent){
	fprintf(stderr, "CreateNode is called\n");
	struct node* new_node = (struct node *)malloc(sizeof(struct node));
	if(strcmp(path,"/"))
		new_node->filename=GetFileNamefromPath(path);
	else
		new_node->filename="";
	new_node->path=path;
	new_node->type=type;

	//Permissions temporarily set to normal files
	if(type==0)
		new_node->mode=S_IFREG | 00400;
	if(type==1)
		new_node->mode=S_IFDIR | 00400;
	new_node->parent=parent;
	new_node->child_count=0;
	
	//set time
	time_t now = time(NULL);
	struct tm *timeinfo = gmtime(&now);
	new_node->atime = timeinfo->tm_sec;
	new_node->mtime = timeinfo->tm_sec;
	new_node->ctime = timeinfo->tm_sec;

	//
	nodes[file_num]=new_node;
	file_num++;
	return new_node;
}
void PrintNode(struct node* node){
	if (node == NULL) {
        printf("Error: node is NULL\n");
        return;
    }
	fprintf(stderr, "PrintNode is called\n");
	if (node->filename!=NULL) printf("filename: %s\n", node->filename);
	else printf("filename: \n");
	if (node->contents!=NULL) printf("contents: %s\n", node->contents);
	else printf("contents: \n");
	if (node->path!=NULL) printf("path: %s\n", node->path);
	else printf("path: \n");
	printf("child_count: %d\n", node->child_count);
	if (node->type!=NULL) printf("type: %d\n", node->type);
	else printf("type: \n");
	if(node->mode!=NULL) printf("mode: %d\n", node->mode);
	else printf("mode: \n");
}

struct node* FindNode(const char *path){
	fprintf(stderr, "FindNode is called,tring to find %s\n",path);

	for(int i=0;i<file_num;i++){
		if(!strcmp(nodes[i]->path,path)){
			printf("Lookup successed, %s was found.\n",nodes[i]->path);
			PrintNode(nodes[i]);
			return nodes[i];
		}
	}
	printf("Lookup failed, %s does not exist in the file system.\n",path);
	return NULL;
}
void FindChild(struct node* parent,char** child_names){
	fprintf(stderr, "FindChild is called\n");
	if(parent->child_count==0){
		printf("path %s has no child\n",parent->path);
		return ;
	}

	//int child_count=parent->child_count;
	for(int i=0;i<parent->child_count;i++){
		strcpy(child_names[i],parent->children[i]->filename);
	}

}


#define OPTION(t, p)                           \
    { t, offsetof(struct options, p), 1 }
static const struct fuse_opt option_spec[] = {
	OPTION("--name=%s", filename),
	OPTION("--contents=%s", contents),
	OPTION("-h", show_help),
	OPTION("--help", show_help),
	FUSE_OPT_END
};

void* my_init(struct fuse_conn_info *conn, struct fuse_config *cfg)
{
    fprintf(stderr, "my_init is called"); 
	//create root node
	
	nodes[file_num]=CreateNode("/",1,NULL);
	/*PrintNode(nodes[0]);
	FindNode("/");*/
    return NULL;
}

int my_mknod(const char *path, mode_t m)
{
	fprintf(stderr, "my_mknod path = %s\n", path); 
    struct options nod;
	nod.filename=strdup("new_file");
	//options_list[0]=nod;
    return 0;
}
int my_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
    // Create a new file in memory
	fprintf(stderr, "my_create path = %s\n", path); 
	/*if(mode==)
	CreateNode(path,)*/
    return 0;
}

int my_open(const char *path, struct fuse_file_info *fi)
{
	fprintf(stderr, "my_open path = %s\n", path); 
	syslog(LOG_INFO, "my_open is called, path is %s", path);
	if(!strcmp(path,"/filename")){
		return 0;
	}
	if(!strcmp(path,"/new_file")){
		return 0;
	}
    return -ENOENT;
}

int my_write(const char *path, const char * buf, size_t len, off_t offset,struct fuse_file_info *fi)
{
	fprintf(stderr, "my_write path = %s\n", path); 
	options_list[0].contents="Hello World";
    return 0;
}


int my_truncate(const char *path, off_t length){
	fprintf(stderr, "my_truncate path = %s\n", path); 
	return 0;
}

static int my_getattr(const char *path, struct stat *stbuf,
			 struct fuse_file_info *fi)
{
	fprintf(stderr, "my_getattr path = %s\n", path); 
	//stbuf->st_mode = S_IFDIR | 00400;
	//int res = 0;
	struct node* target_node=FindNode(path);
	printf("********************");
	PrintNode(target_node);
	if(FindNode(path)==NULL){
		printf("my_getattr failed,path=%s\n",path);
		return -ENOENT;	
	}
	else{
		stbuf->st_mode=target_node->mode;
		//stbuf->st_mode = S_IFDIR | 00400;
		
		return 0;
	}
	
	//struct node* target_node=FindNode(path);
	//return 0;
	//PrintNode(target_node);
	/*if(target_node){
		if(target_node->type==0){
			//stbuf->st_mode = target_node->mode;
		}
		else if(target_node->type==1){
			//printf("target_node->mode: %d\n", target_node->mode);
			//stbuf->st_mode = target_node->mode;
			//stbuf->st_mode = S_IFDIR | 00400;
			//stbuf->st_size=strlen(target_node->contents);
		}
		stbuf->st_atime = target_node->atime;
		stbuf->st_mtime = target_node->mtime;
		stbuf->st_ctime = target_node->ctime;
		return 0;

	}
	else{
		printf("my_getattr failed,path=%s\n",path);
		return -ENOENT;
	}*/
		
}

static int my_read(const char *path, char *buf, size_t size, off_t offset,
		      struct fuse_file_info *fi)
{
	fprintf(stderr, "my_read path = %s\n", path); 
	if(!strcmp(path,"/filename")){
		strcpy(buf,"Hello World\n");
		return strlen("Hello World\n");
	}
	return -ENOENT;
}

static int my_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info *fi,
			 enum fuse_readdir_flags flags)
{
	fprintf(stderr, "my_readdir path = %s\n", path); 

	filler(buf, ".", NULL, 0, 0);
	filler(buf, "..", NULL, 0, 0);

	//Find child nodes under the path
	struct node* parent=FindNode(path);
	//char** child_names[MAX_CHILDREN];
	char **child_names = (char **)malloc(MAX_CHILDREN * sizeof(char *));
	FindChild(parent,child_names);
	int i=0;
	/*while(child_names[i]!=NULL){
		filler(buf, child_names[i], NULL, 0, 0);
		i++;
	}*/
	free(child_names);
	return 0;
}
/*
my_utimens函数来更新文件的时间戳
*/
int my_utimens(const char *path, const struct timespec tv[2], struct fuse_file_info *fi) {
	fprintf(stderr, "my_utimens path = %s\n", path); 
	return 0;
}
int my_setattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi)
{
	fprintf(stderr, "my_setattr path = %s\n", path); 
    int res = 0;

    return res;
}
int my_release(const char *path, struct fuse_file_info *fi) {
	fprintf(stderr, "my_release path = %s\n", path); 
    return 0;
}


static const struct fuse_operations hello_oper = {
	.init=my_init,
	.mknod=my_mknod,
	.create=my_create,
	.open=my_open,
	.write=my_write,
	.release=my_release,
	//.truncate=my_truncate,
	.getattr	= my_getattr,
	.setxattr=my_setattr,
	.utimens=my_utimens,
	.readdir	= my_readdir,
	.read=my_read,
};

static void show_help(const char *progname)
{
	printf("usage: %s [options] <mountpoint>\n\n", progname);
	printf("File-system specific options:\n"
	       "    --name=<s>          Name of the \"hello\" file\n"
	       "                        (default: \"hello\")\n"
	       "    --contents=<s>      Contents \"hello\" file\n"
	       "                        (default \"Hello, World!\\n\")\n"
	       "\n");
}

int main(int argc, char *argv[])
{
	printf("main\n");
	fflush(stdout);

	int ret;
	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

	if (fuse_opt_parse(&args, &options, option_spec, NULL) == -1)
		return 1;

	if (options.show_help) {
		show_help(argv[0]);
		assert(fuse_opt_add_arg(&args, "--help") == 0);
		args.argv[0][0] = '\0';
	}

	ret = fuse_main(args.argc, args.argv, &hello_oper, NULL);
	fuse_opt_free_args(&args);
	return ret;
}
