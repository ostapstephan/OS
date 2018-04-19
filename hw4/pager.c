#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/resource.h>
#include <ctype.h>

int main(int argc, char* argv[]){
    size_t size= 256;
    size_t sIn=256;
    char *line=NULL;
    char * buf = malloc(sIn);
    int fd;    
    int i=0; 
    int br=2;
    if((fd = open("/dev/tty",O_RDONLY))<0){
        fprintf(stderr,"Error opening /dev/tty for reading: %s\n",strerror(errno));
        return -1;
    } 
    while((br=getline(&line,&size,stdin))!=-1){
        i=0; 
        if (br==-1){
            break;
        }
        fprintf(stdout,"%s",line);
        while(((br=getline(&line,&size,stdin))==-1)||i<22){
            if (br==-1){
                break;
            }
            fprintf(stdout,"%s",line);
            i++;
        }
        if (br==-1){
            break;
        }//if it breaks outta the first it should break outta the second
        fprintf(stdout,"PRESS RETURN TO CONTINUE\n");
        if (read(fd,buf,sIn)<0){
            fprintf(stderr,"Error reading from /dev/tty: %s\n",strerror(errno));
        }
        if ((buf[0]=='q')||(buf[0]=='Q')||(br==0)){
            break; 
        }
    } 
}
