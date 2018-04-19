#include <sys/mman.h>
#include <sys/types.h>
#include <wait.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <stdlib.h>
#include <signal.h>
#include "spinlock.h"
#include "cv.h"

volatile sig_atomic_t usr_interrupt= 0;
struct sigaction act;
sigset_t mask,oldmask;

void hdlsig(int k){//done
    usr_interrupt=1;
}

void cv_init(struct cv *cv){//done
    spin_unlock(&cv->sp);
    memset(cv->pArr,0,CV_MAXPROC); 
    cv->filled=0; 
    act.sa_handler=hdlsig;
    sigaction(SIGUSR1, &act,NULL); //set up mask of signals to temporarily block   
    sigemptyset(&mask);
    sigaddset(&mask,SIGUSR1); 
}

void cv_wait(struct cv *cv,struct spinlock *mutex){

    spin_lock(&(cv->sp));    //wait for signal to arrive  
    usr_interrupt=0;
    
    cv->sp= *mutex;
    cv->pArr[cv->filled]=(int)getpid();
    //fprintf(stderr, "(%d) Added to CV on %d\n", getpid(), cv->filled);
    cv->filled++;

    /*for (int j =0; j<CV_MAXPROC;j++){*/
        /*if(cv->pArr[j]!=0 ){*/
            /*cv->pArr[j]=getpid();*/
            /*cv->filled++;*/
            /*break;*/
        /*} */
    /*}*/

    sigprocmask(SIG_BLOCK,&mask,&oldmask); 
    spin_unlock(mutex);
    spin_unlock(&(cv->sp));
    while(!usr_interrupt) {
        sigsuspend(&oldmask);
    }   
    spin_lock(mutex);
    sigprocmask(SIG_UNBLOCK,&mask,NULL); 
}

int cv_broadcast(struct cv *cv){//done
    //loop through all the struct cv pArr pid's and use kill to 
    //send a SIGUSR1 to all the users so they know to wake up  
    spin_lock(&cv->sp);
    int sleepers=0;
    for(int i=0; i<cv->filled;i++ ){ //loop through all pids 
        if (cv->pArr[i]>0){//if there is a pid there send it a sig
            sleepers++;
            kill(cv->pArr[i],SIGUSR1);
        }    
    }
    cv->filled =0;
    spin_unlock(&cv->sp);
    return sleepers;//return # woken sleepers
}

int cv_signal(struct cv *cv){//done
    int ret=0;
    spin_lock(&cv->sp);
     
    if (cv->filled>0){ 
        //fprintf(stderr, "(%d) Removed from CV[%d]\n", getpid(), cv->filled-1);
        kill(cv->pArr[cv->filled-1],SIGUSR1);
        ret++;
        cv->filled--; 
    }
    spin_unlock(&cv->sp);
    return ret;
}


