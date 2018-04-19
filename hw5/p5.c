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
    char *filename = "r5";
    int fd=0;
    size_t length = 0;//sizeof(fd)
    struct stat st;
    void *mmapped;
    size_t len=1;
    char buf[len]; 
    char buf2[len]; 
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
    lseek(fd,length+15,SEEK_SET);
    *buf='B';
    write(fd,buf,len);
    stat(filename,&st);
    lseek(fd,length+1,SEEK_SET);
    read(fd,buf2,len);
    fprintf(stderr,"Old size %lu, new size %lu \n", length, st.st_size);
    if (*buf2 == 'R'){
        fprintf(stderr,"The byte written is now visible\n");
        close(fd);
        return 0;
    }
    fprintf(stderr,"The byte written is not visible\n");
    close(fd);
    return 1;
}
