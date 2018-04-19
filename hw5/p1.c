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

void hdlsig(int k){
    fprintf(stderr, "program exited with signal: %s\n",strsignal(k));
    exit(k); 
}

int main(int argc, char* argv[]){
    char *fn = "r1"; 
    int fd;
    size_t length = 0;//sizeof(fd)
    struct stat st;
    void *mmapped;
    char *garbageData = "HEY THIS IS WHAT I WANT TO WRITE- ostap";
    for(int i=0;i<32;i++){//loop through all of the signals to make sure that you handle them all
        signal(i, hdlsig); 
    }
    genfile(fn);//generate file with file name passed
    fd = open(fn,O_RDONLY);//open file name with file name passed
    stat(fn,&st);
    length = st.st_size; 
    mmapped = mmap(NULL, length, PROT_READ, MAP_SHARED ,fd, 0);//null for kernel to decide, length of file, prot read obvious, mapshared so its not shared, fd is fd of file to write, 0 offset so entire file is written
    if(mmapped==MAP_FAILED){
        fprintf(stderr,"Error attempting to MMAP file %s: %s\n",fn,strerror(errno));
        exit (-1);
    }
    //copy garbageData to the mmapped region 
    memcpy(mmapped,garbageData,strlen(garbageData)); 
    fprintf(stderr, "No errors\n");
    close(fd);
    return 0;
}
