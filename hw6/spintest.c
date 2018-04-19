#include <sys/mman.h>
#include <wait.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <stdlib.h>
#include "spinlock.h"

int main(int argc, char* argv[]){  
    int *v;
    int nolock;
    int status;
    pid_t pid;
    pid_t forkPid;
    int nChild =atoi(argv[1]);  
    int niter = atoi(argv[2]);
    v=0;
    //---------mmap shared region-----------
    v = mmap(NULL, 4096,PROT_READ|PROT_WRITE , MAP_SHARED|MAP_ANON, -1, 0);
    if(v == MAP_FAILED){
        exit(EXIT_FAILURE);   
    }
    struct spinlock* L = mmap(NULL, 4096,PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);
    if(v == MAP_FAILED){
        exit(EXIT_FAILURE);   
    }
    L->lock= 0;



    //-------start of forks and --------------- 
    for(int i =0; i<nChild; ++i){
        forkPid=fork();
        if (forkPid==0){
            //do the child thing with ho lock

            int temp = *v;
            for(int j=0;j<niter;j++){
                temp ++;
            }
            *v= temp;
            //end child thing and exit 
            exit(0);
        }
        else if(forkPid==-1){
            fprintf(stderr,"Error, fork failed");
            exit(EXIT_FAILURE);
        }
    }

    for(int i=0; i<nChild; ++i){
        pid = wait(&status);
        fprintf(stderr,"child %i returned: %i \n",pid,status);
    }

    nolock = *v;
    *v=0;
    // -----reset and try again with lock------
    for(int i =0; i<nChild; ++i){
        forkPid=fork();
        if (forkPid==0){
            //do the child thing with  lock
            spin_lock(&(*L));
            int temp = *v;
            for(int j=0;j<niter;j++){
                temp ++;
            }
            *v= temp;
            spin_unlock(&(*L));
            //end child thing and exit 
            exit(0);
        }
        else if(forkPid==-1){
            fprintf(stderr,"Error, fork failed");
            exit(EXIT_FAILURE);
        }
    }

    for(int i=0; i<nChild; ++i){
        pid = wait(&status);
        fprintf(stderr,"child %i returned: %i \n",pid,status);
    }


    fprintf(stderr, "this is the val of the variable with no lock:\t\t\t%i \nThis is the value of the variable with spinlock enabled:\t%i  \nThis is the value it should be \t\t\t\t\t%i \n",nolock,*v, (nChild*niter) );
return 1;
}


