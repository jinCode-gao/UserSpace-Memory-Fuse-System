//Used to test the disk write speed of the linux file system

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    int fd = open("f1", O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    char *buf = "gLorem ipsum dolor sit amet, consectetur adipiscing elit. Integer placerat, dolor vitae blandit viverra, est leo sollicitudin sapien, id tempor urna lectus eget magna. Nulla facilisi. Duis auctor, lacus at tempor convallis, lacus nulla ullamcorper enim, quis fringilla mi mi eget est. Sed tristique tellus euismod, bibendum nulla ac, placerat est. Praesent aliquet, quam ac feugiat tempor, nibh lacus bibendum ligula, eu tincidunt magna risus vel ipsum. Integer vel commodo velit. Aenean id tempor nisi, non commodo enim. Sed egestas congue libero, auctor blandit dui consectetur vel. Sed euismod aliquet ante eget bibendum.Etiam vel risus vel odio pharetra accumsan. Maecenas in est tempor, molestie enim vel, auctor metus. Praesent congue, orci vel consectetur laoreet, lectus leo malesuada mauris, non tempor erat purus in sapien. Integer volutpat justo non nisi tempor, eu euismod enim consectetur. Curabitur suscipit nunc a tellus bibendum, id posuere sapien euismod. auctor blandit dui cgLorem ipsum dolor sit amet, consectetur adipiscing elit. Integer placerat, dolor vitae blandit viverra, est leo sollicitudin sapien, id tempor urna lectus eget magna. Nulla facilisi. Duis auctor, lacus at tempor convallis, lacus nulla ullamcorper enim, quis fringilla mi mi eget est. Sed tristique tellus euismod, bibendum nulla ac, placerat est. Praesent aliquet, quam ac feugiat tempor, nibh lacus bibendum ligula, eu tincidunt magna risus vel ipsum. Integer vel commodo velit. Aenean id tempor nisi, non commodo enim. Sed egestas congue libero, auctor blandit dui consectetur vel. Sed euismod aliquet ante eget bibendum.Etiam vel risus vel odio pharetra accumsan. Maecenas in est tempor, molestie enim vel, auctor metus. Praesent congue, orci vel consectetur laoreet, lectus leo malesuada mauris, non tempor erat purus in sapien. Integer volutpat justo non nisi tempor, eu euismod enim consectetur. Curabitur suscipit nunc a tellus bibendum, id posuere sapien euismod. auctor blandit dui c";
    int len = strlen(buf);

    
    int ret = write(fd, buf, len);
    clock_gettime(CLOCK_MONOTONIC, &end);
    if (ret < 0) {
        perror("write");
        return 1;
    }

    printf("time elapsed: %ld ns\n", (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec));

    close(fd);

    return 0;
}
