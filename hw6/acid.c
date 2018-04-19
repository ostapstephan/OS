#include "fifo.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <wait.h>
#include <string.h>
#include <errno.h>

int main(int argc, char* argv[]){
    int writers,status,items;
    pid_t forkPid, pid;
    struct fifo *f;

    writers = atoi(argv[1]);
    items = atol(argv[2]);

    fprintf(stderr, "(%d) PARENT\n", getpid());

    if (argc!=3|| (writers)<=0 || (items)<=0){
        fprintf(stderr, "Error incorrect argument count, exiting");
        exit (-1);
    }

    f= mmap(NULL, sizeof(struct fifo),PROT_READ|PROT_WRITE , MAP_SHARED|MAP_ANON, -1, 0);
    //check if mmap is successful
    if (f==MAP_FAILED){
        fprintf(stderr,"Error fifo MMap failed: %s\n",strerror(errno));
        exit(-1);
    }
    //init the fifo 
    fifo_init(f);

    //writer
    for (int i =0;i<writers; i++){
        //fprintf(stderr,"(%d) Forking %d\n", getpid(), i);
        forkPid=fork();
        if (forkPid==0){
            //do the child thing
                fprintf(stderr, "(%d) CHILD_%d\n", getpid(), i);
            for(unsigned long j=0; j<items; j++){
                fifo_wr(f,j);
            }
            //end child thing and exit 
            exit(0);
        }
        else if(forkPid==-1){
            fprintf(stderr,"Error, fork failed");
            exit(-1);
        }
    }
    //reader 
    fprintf(stderr,"(%d) Fork (writers) complete\n", getpid());
    forkPid=fork();
    unsigned long rul;//read unsigned long
    if (forkPid==0){
        //do the child reader thing
        fprintf(stderr,"(%d) READ_CHILD\n",getpid());
        for(int k=0; k<writers*items; k++){
            rul= fifo_rd(f);
        }
        //end child thing and exit 
        exit(0);
    }
    else if(forkPid==-1){
        fprintf(stderr,"Error, fork failed");
        exit(-1);
    }


    for(int i=0; i<writers+1; i++){
        pid = wait(&status);
        fprintf(stderr,"(%d) returned %i\n",pid,status);
    }
    return 1;
}
