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

#define OPTION(t, p)                           \
    { t, offsetof(struct options, p), 1 }
static const struct fuse_opt option_spec[] = {
	OPTION("--name=%s", filename),
	OPTION("--contents=%s", contents),
	OPTION("-h", show_help),
	OPTION("--help", show_help),
	FUSE_OPT_END
};

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

int my_release(const char *path, struct fuse_file_info *fi)
{
	fprintf(stderr, "my_release path = %s\n", path);
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
	int res = 0;
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 00400;
		return 0;
	}
	if (strcmp(path, "/filename") == 0) {
		stbuf->st_mode = S_IFREG | 00400;
		stbuf->st_size=strlen("Hello World\n");
		return 0;
	}
	if (strcmp(path, "/new_file") == 0) {
		stbuf->st_mode = S_IFREG | 00400;
		stbuf->st_size=0;

	}
	return -ENOENT;
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
	syslog(LOG_INFO, "my_readdir is called, path is %s", path);

	filler(buf, ".", NULL, 0, 0);
	filler(buf, "..", NULL, 0, 0);
	filler(buf,"filename", NULL, 0, 0);

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

static const struct fuse_operations hello_oper = {
	.mknod=my_mknod,
	.create=my_create,
	.open=my_open,
	.write=my_write,
	//.release=my_release,
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
