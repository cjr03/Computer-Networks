// COEN 146L : Lab 1 - template to use for file transfer (steps 3, 4, and 5)
// You need to adapt this template according to the requirements of each of the steps
#include <stdio.h> // fprintf(), fread(), fwrite(), fopen(), fclose()
#include <stdlib.h> // malloc(), free()
#include <pthread.h> // pthread_create()
#include <unistd.h> // read(), write()
#include <fcntl.h> // open(), close()
#include <errno.h> // errno
#include <time.h> // clock()
#include <sys/mman.h> // mmap()
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
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

// copies a files form src_filename to dst_filename using syscalls open(), read(), write(), close()
int syscall_copy(char* src_filename, char* dst_filename) {
    int src_fd = open(src_filename, O_RDONLY); // opens a file for reading
    if (src_fd < 0) { // open() error checking
        fprintf(stderr, "unable to open %s for reading: %i\n", src_filename, errno);
        close(src_fd);
        exit(0);
    }
    // O_WRONLY - write only
    // O_CREAT - create file if it doesn't exist
    // O_TRUNC - truncates file to length 0 if it already exists (deletes existing file)
    // opens a file for writing; erases old file/creates a new file
    int dst_fd = open(dst_filename, O_WRONLY | O_CREAT | O_TRUNC);
    if (dst_fd < 0) { // open() error checking
        fprintf(stderr, "unable to open/create %s for writing: %i\n", dst_filename, errno);
        close(dst_fd);
        exit(0);
    }
    
    // read/ write loop
    char* buf = malloc((size_t)BUF_SIZE); // allocate a buffer to store read data
    ssize_t bytes_read;
    while((bytes_read = read(src_fd, buf, BUF_SIZE)) > 0){ // reads up to BUF_SIZE bytes from src_filename
        write(dst_fd, buf, bytes_read);  // writes bytes_read to dst_filename
    }
    free(buf);
    close(dst_fd);
    close(src_fd);
    return 0;
}

// checks the runtime to copy a file using functions vs syscalls
void check_copy_times(char* src_filename, char* dst_filename) {
    clock_t func_start, func_end, syscall_start, syscall_end;
    syscall_start = clock();
    syscall_copy(src_filename, dst_filename);
    syscall_end = clock();
    func_start = clock();
    func_copy(src_filename, dst_filename);
    func_end = clock();
    // capture runtime of func_copy() using start clock, call the copy, end the clock
    // capture runtime of syscall_copy() using start clock, call the copy, end the clock
    printf("time to copy using functions: %.7f\n", ((double)func_end-func_start)/CLOCKS_PER_SEC);
    printf("time to copy using syscalls: %.7f\n", ((double)syscall_end-syscall_start)/CLOCKS_PER_SEC);
}

int main(int argc, char* argv[]) {
    if (argc != 3) { // check correct usage
        fprintf(stderr, "usage: %s <src_filename> <dst_filename>\n", argv[0]);
        exit(1);
    }
    char * src = argv[1], * dst = argv[2];
    check_copy_times(src, dst);
    //get the source and destination files from the command line arguments
    //call the check copy times function
    return 0;
}

