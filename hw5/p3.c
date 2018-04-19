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
    char *filename = "r3";
    int fd=0;
    size_t length = 0;//sizeof(fd)
    struct stat st;
    void *mmapped;
    size_t len=1;
    char buf[len]; 
    genfile(filename);
    fd = open(filename,O_RDWR);
    if(fd==-1){
        fprintf(stderr,"Error attempting to open file %s for Reading and Writing: %s\n",filename,strerror(errno));
        exit (-1);
    }
    stat(filename,&st);
    length = st.st_size; 
    mmapped = mmap(NULL, length, PROT_READ|PROT_WRITE , MAP_PRIVATE ,fd, 0);//null for kernel to decide, length of file, prot read obvious, mapshared so its not shared, fd is fd of file to write, 0 offset so entire file is written 
    if(mmapped==MAP_FAILED){
        fprintf(stderr,"Error attempting to MMAP file %s: %s\n",filename,strerror(errno));
        exit (-1);
    }
    //checking the first byte of the pointer mmapped will match the file untill
    //a write is made via any means 
    *(char*)mmapped = 'R';
    read(fd, buf, len); 
    if(*buf == 'R'){
        fprintf(stderr,"Write to MAP_PRIVATE changed the original file\n");
        close(fd); 
        return 0;//successful write 
    }
    fprintf(stderr,"Write to MAP_PRIVATE did not change the original file\n");
    close(fd); 
    return 1;//Failed to write so return 1 
}
