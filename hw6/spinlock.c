#ifndef SPIN_C
#define SPIN_C


#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include "spinlock.h"


void spin_lock(struct spinlock *l){
    //fprintf(stderr,"spin_lock\n");
    int rY =0;// return yield
    while(tas(&l->lock)!=0){//check lock var
        if ((rY=sched_yield())!=0){//sched_yield only ret 0 on success 
            fprintf(stderr, "Error attempting to sched_yield() in process %i\n%s\n",getpid(), strerror(errno));  
        }
    }// wait until we have lock access 
    l->pidCurr = getpid();
    //fprintf(stderr,"spin_lock-exit\n");
}

void spin_unlock(struct spinlock *l){
    //fprintf(stderr,"spin_unlock\n");
    l->lock=0; 
    //fprintf(stderr,"spin_unlock-exit\n");
}

#endif //SPIN_C
