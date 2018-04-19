#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <wait.h>
#include <sys/mman.h>
#include "spinlock.h"
#include "cv.h"
#include "fifo.h"


void fifo_init(struct fifo *f){
    spin_unlock(&f->mutex);//unlock mutex lock 
    cv_init(&f->full);  //  init your condition vars with 
    cv_init(&f->empty); //  the proper funtion cv_init
    f->next_read=0;     //  set all vals to 0
    f->next_write=0; 
    f->itemCount=0;  
    memset(&f->buf,0,CV_MAXPROC);// clear the buffer and set to 0 
}

void fifo_wr(struct fifo *f, unsigned long d){
    spin_lock(&(f->mutex));
    
    while (f->itemCount>=MYFIFO_BUFSIZE){//
        //fprintf(stderr, "(%d) fifo is full\n", getpid());
        cv_wait(&f->full,&f->mutex);
    }
    f->buf[f->next_write++]=d;
    f->next_write%=MYFIFO_BUFSIZE;
    f->itemCount++;
    cv_signal(&f->empty);
    spin_unlock(&f->mutex);
}

unsigned long fifo_rd(struct fifo *f){
    unsigned long d; 
    spin_lock(&f->mutex);   
    while(f->itemCount<=0){//empty
        //fprintf(stderr, "(%d) fifo is empty\n", getpid());
        cv_wait(&f->empty,&f->mutex);
    }
    d=f->buf[f->next_read++];
    f->next_read%=MYFIFO_BUFSIZE;
    f->itemCount--;
    cv_signal(&f->full);
    spin_unlock(&f->mutex);
    return d;
}

