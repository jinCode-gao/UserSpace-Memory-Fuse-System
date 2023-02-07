//Used to test the disk read speed of the linux file system

#include <stdio.h>
#include <time.h>

int main()
{
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    FILE *fp;
    char buffer[1024];
    int read_size;

    // Open the file for reading
    fp = fopen("file_read", "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        return -1;
    }

    
    // Read data from the file into the buffer
    read_size = fread(buffer, 1, 1024, fp);


    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("time elapsed: %ld ns\n", (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec));

    // Close the file
    fclose(fp);
    return 0;
}
