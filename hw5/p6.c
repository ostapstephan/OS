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
#include "genfilesmall.h"

void hdlsig(int k){
    fprintf(stderr, "program exited with signal %d: %s\n",k,strsignal(k));
    exit(k); 
}

int main(int argc, char* argv[]){
    char *filename = "r6";
    int fd=0;
    size_t length = 0;//sizeof(fd)
    struct stat st;
    void *mmapped;
    size_t len=1;
    /*int readRet; */
    char buf[len]; 
    /*char buf2[len]; */
    for(int i=0;i<32;i++){//handle all signals
        signal(i, hdlsig); 
    }
    genSFile(filename);//small file less than one page size
    fd = open(filename,O_RDWR);
    if(fd==-1){
        fprintf(stderr,"Error attempting to open file %s for Reading and Writing: %s\n",filename,strerror(errno));
        exit (-1);
    }
    stat(filename,&st);
    length = st.st_size;//length of the size of the file  
    mmapped = mmap(NULL, 8192, PROT_READ|PROT_WRITE , MAP_SHARED ,fd, 0);//2 page mmap
    if(mmapped==MAP_FAILED){//error check mmap
        fprintf(stderr,"Error attempting to MMAP file %s: %s\n",filename,strerror(errno));
        exit (-1);
    } 
   
    *buf= *(char*)(mmapped+length+7); 
    fprintf(stderr,"Byte Value read from 1st page:%d \n",*buf);

    *buf= *(char*)(mmapped+length+4097); 
    fprintf(stderr,"Byte Value read from 2nd page:%d \n",*buf);
    close(fd);
    return 0;
}
