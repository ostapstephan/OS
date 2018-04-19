#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>
#include "genfile.h"


int main(int argc, char* argv[]){
    char *filename = "r4";
    int fd=0;
    size_t length = 0;//sizeof(fd)
    struct stat st;
    void *mmapped;
    genfile(filename);
    fd = open(filename,O_RDWR);
    if(fd==-1){
        fprintf(stderr,"Error attempting to open file %s for Reading and Writing: %s\n",filename,strerror(errno));
        exit (-1);
    }
    stat(filename,&st);
    length = st.st_size; 
    mmapped = mmap(NULL, length, PROT_READ|PROT_WRITE , MAP_SHARED ,fd, 0);//null for kernel to decide, length of file, prot read obvious, mapshared so its not shared, fd is fd of file to write, 0 offset so entire file is written 
    if(mmapped==MAP_FAILED){
        fprintf(stderr,"Error attempting to MMAP file %s: %s\n",filename,strerror(errno));
        exit (-1);
    }

    *(char*)(mmapped+length+1) = 'R';
    stat(filename,&st);
    fprintf(stderr,"Old size %lu, new size %lu\n", length, st.st_size);
    if (st.st_size != length){
        close(fd); 
        return 0;
    }
    close(fd);
    return 1;

}
