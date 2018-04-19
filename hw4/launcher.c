#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>
#include <time.h>
#include <sys/resource.h>
#include <string.h>

int main(int argc, char* argv[]){
    pid_t pid1;
    pid_t pid2;
    pid_t pid3;
    int s1,s2,s3;
    char *wgArg[] = {"./wordgen", argv[1] , NULL};
    char *wsArg[] = {"./wordsearch", "l3.txt", NULL};
    char *pgArg[] = {"./pager", NULL};
    int pfd1[2];
    int pfd2[2];
    pipe(pfd1); 
    pipe(pfd2);
    switch(fork()){
        case -1:{//error occured
                    fprintf(stderr,"Error when attempting to fork for ./wordgen: %s",strerror(errno));
                    return -1;
                } 
        case 0:{//in first child ./wordgen
                   dup2(pfd1[1],1);//stdout->write end of pipe1
                   close(pfd1[0]);
                   close(pfd1[1]); 
                   close(pfd2[0]);
                   close(pfd2[1]); 
                   execvp(*wgArg, wgArg);
               } 
        default:{//parent ./launcher
                    switch(fork()){
                        case -1:{//error occured
                                    fprintf(stderr,"Error when attempting to fork for ./wordsearch: %s",strerror(errno));
                                    return -1;
                                }
                        case 0:{ //in child 2
                                   dup2(pfd1[0],0);//stdin->read end of pipe1
                                   dup2(pfd2[1],1);//stdout->write end of pipe2
                                   close(pfd1[0]);
                                   close(pfd1[1]); 
                                   close(pfd2[0]);
                                   close(pfd2[1]); 
                                   execvp(*wsArg, wsArg);
                               }
                        default:{//parent 2
                                    switch(fork()){
                                        case -1:{//error occured
                                                    fprintf(stderr,"Error when attempting to fork for ./pager: %s",strerror(errno));
                                                    return -1;
                                                }
                                        case 0:{//in child 3
                                                   dup2(pfd2[0],0);//stdin->read end of pipe2
                                                   close(pfd1[0]);
                                                   close(pfd1[1]); 
                                                   close(pfd2[0]);
                                                   close(pfd2[1]); 
                                                   execvp(pgArg[0], pgArg);
                                               }
                                    }//fork switch 3
                                }//end of default 2 
                    }//fork switch 2
                }//end of default 1
    }//fork switch 1
    close(pfd1[0]);
    close(pfd1[1]); 
    close(pfd2[0]);
    close(pfd2[1]); 
    
    pid3=wait(&s3);
    fprintf(stdout,"child %i returned %i \n",pid3,s3);
   
    pid2=wait(&s2);
    fprintf(stdout,"child %i returned %i \n",pid2,s2);
   
    pid1=wait(&s1);
    fprintf(stdout,"child %i returned %i \n",pid1,s1);
}//end of main
