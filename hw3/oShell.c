#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>
#include <time.h>
#include <sys/resource.h>

void ReDirect(char **redVec,int numR){
    int in;
    int out;
    int err;
    //check to be able to open file 
    for(int i=0;i<numR;i++){
        if(redVec[i][0]=='<'){
            redVec[i]=&redVec[i][1];
            fprintf(stderr,"%s\n",redVec[i]); 
            if( (in = open(redVec[i],O_RDONLY))<0){
               fprintf(stderr,"ERROR opening file %s for reading: %s\n",redVec[i],strerror(errno));
            }
            dup2(in,0);
        }
        else if(redVec[i][0]=='>'){
            if(redVec[i][1]=='>'){
                redVec[i]=&redVec[i][2];
                if((out = open(redVec[i],O_WRONLY|O_CREAT|O_APPEND,0666))<0){
                    fprintf(stderr,"ERROR opening file %s for writing: %s\n",redVec[i],strerror(errno));
                }
                dup2(out,1);
            }            
            else{
                redVec[i]=&redVec[i][1];
                if((out = open(redVec[i],O_WRONLY|O_CREAT|O_TRUNC,0666))<0){
                    fprintf(stderr,"ERROR opening file %s for writing: %s\n",redVec[i],strerror(errno));
                }
                dup2(out,1);
            }
        }
        else if(redVec[i][0]=='2'&&redVec[i][1]=='>'){
            if( redVec[i][2]=='>'){
                redVec[i]=&redVec[i][3];
                err = open(redVec[i],O_WRONLY|O_CREAT|O_APPEND,0666);
                dup2(err,2);
            }
            else{
                redVec[i]=&redVec[i][2];
                if((err = open(redVec[i],O_WRONLY|O_CREAT|O_TRUNC,0666))==-1){
                fprintf(stderr,"Error opening %s: %s /n",redVec[i],strerror(errno));}
                dup2(err,2);
            }
        }
    }
}

int readin(char* fn,char** myArg,FILE* inFile){  //finish this argument bs
    struct timespec start,stop;
    struct rusage ru;
    size_t sizeBuf=4096; 
    char* token; 
    char delim[]=" \t";  
    char *buf=NULL;
    int splSize=2048;
    char* argline[splSize];//Split Line Array of pointers to chars
    char* redirs[4]={NULL,NULL,NULL,NULL};
    int read=0; 
    int iArg=0;
    int r[3]={0,0,0}; 
    int rind=0;
    int ii=0;
    int status;
    pid_t pid;
    
    //getline and check for EOF 
    if((read=getline(&buf,&sizeBuf,inFile))==-1){
        fprintf(stderr,"End of File encountered \n");
        return 0;  
    }
    buf[read-1] ='\0';//remove that pesky newline
    //tokenize 
    token = strtok(buf,delim);//gets first token
    while(token){
        if((token[0]=='<')||(token[0]=='>')||(token[0]=='2'&&token[1]=='>')){//put all redirects into redirect array after their first encounter
            while(token){
                if(!((token[0]=='<')||(token[0]=='>')||(token[0]=='2'&&token[1]=='>'))){
                    fprintf(stderr,"Error reading redirects. There should not be any non redirects after the first redirect");
                    return(-1);
                }          
                if ((token[0]=='<')){r[0]++;}
                if((token[0]=='>')){r[1]++;}
                if((token[0]=='2'&&token[1]=='>')){r[2]++;}
                if(r[0]>1||r[1]>1||r[2]>1){
                    fprintf(stderr,"error too Many Redirects:%d,token:%s",rind,token);
                    return(-1);
                }
                redirs[rind]=token; 
                redirs[rind+1] = NULL;
                rind++;
                token=strtok(NULL,delim); 
            } 
        }
        else{
            argline[iArg]=token;
            argline[iArg+1]=NULL; 
            iArg++;
            token = strtok(NULL,delim);//continues getting new tokens
        }
    }  
    argline[iArg]=NULL;
    
    ii=0;
    
    if ((strcmp(argline[0],"cd"))==0) {
        chdir(argline[1]);
        return 1;
    }
    else if(argline[0][0]=='#'){
        return 1;
    }
    else if(strcmp(argline[0],"exit")==0){
        exit(atoi(argline[1]));
    }

    //=================================================
    //==============FORK EXEC AND DUP================== 
    //=================================================
    
    switch (pid=fork()){
        case -1:
            //error occurred 
            fprintf(stderr,"Error, failed to fork");
            return -1;
        case 0:
            //in child
            ReDirect(redirs,rind);
            if(execvp(argline[0],argline)<0){
                fprintf(stderr,"Error, '%s' Failed attempting to execvp: %s ",argline[0],strerror(errno));
            }

        default:
            if(clock_gettime(CLOCK_REALTIME, &start) == -1){
                fprintf(stderr,"Error getting the real start time: %s\n",strerror(errno));
            } 
            wait3(&status,0,&ru);
            if(clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
                fprintf(stderr,"Error getting the real end time: %s\n",strerror(errno));
            } 
            else{ 
                fprintf(stdout,"User Time: %ld.%06ld seconds\n",(long)ru.ru_utime.tv_sec, (long)ru.ru_utime.tv_usec);
                fprintf(stdout,"System Time: %ld.%06ld seconds\n",(long)ru.ru_stime.tv_sec, (long)ru.ru_stime.tv_usec);
                fprintf(stdout,"Real Time: %ld.%09ld seconds\n",stop.tv_sec-start.tv_sec,stop.tv_nsec-start.tv_nsec);
            } 
    }
    
    ii=0;
    return 1;  
}

int main(int argc, char* argv[]){
    char* fn=NULL;//max fn is 256 char 
    char* args[2048];
    FILE * input; 
    if (argc>1 ){
        if((input = fopen(argv[1],"r"))==NULL){
           fprintf(stderr,"Error occurred opening a shellscript file %s: %s \n",argv[1],strerror(errno)); 
        } 
    }
    else{
        input= stdin;
    }

    while (readin(fn,args,input)>0){
    }
    return 1;
}
