// COEN 146L : Lab 1 - template to use for file transfer (steps 3, 4, and 5)
// You need to adapt this template according to the requirements of each of the steps
#include <stdio.h> // fprintf(), fread(), fwrite(), fopen(), fclose()
#include <stdlib.h> // malloc(), free()
#include <errno.h> // errno
#define BUF_SIZE 2048 //buffer size

// copies a files from src_filename to dst_filename using functions fopen(), fread(), fwrite(), fclose()
int func_copy(char* src_filename, char* dst_filename) {
    FILE *src = fopen(src_filename, "r"); // opens a file for reading
    if (src == NULL) { // fopen() error checking
        fprintf(stderr, "unable to open %s for reading: %i\n", src_filename, errno);
        exit(0);
    }
    FILE *dst = fopen(dst_filename, "w"); // opens a file for writing; erases old file/creates a new file
    if (dst == NULL) { // fopen() error checking
        fprintf(stderr, "unable to open/create %s for writing: %i\n", dst_filename, errno);
        fclose(src);
        exit(0);
    }
    char* buf = malloc((size_t)BUF_SIZE); // allocate a buffer to store read data
    size_t bytes_read;
    while ((bytes_read = fread(buf, 1, BUF_SIZE, src))>0) {
        fwrite(buf, 1, bytes_read, dst);
    }
    free(buf);
    fclose(dst);
    fclose(src);
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 3) { // check correct usage
        fprintf(stderr, "usage: %s <src_filename> <dst_filename>\n", argv[0]);
        exit(1);
    }
    char * src = argv[1], * dst = argv[2];
    func_copy(src, dst); //get the source and destination files from the command line arguments //call the check copy times function
    return 0;
}

