#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

int m=0;
void handlePipe(int k){ 
    fprintf(stderr, "Matched %i words\n",m );
    exit(-1);
}

int main(int argc, char *argv[]){
    FILE* fd;
    size_t dWrdS=1024;
    char * dic[512000];
    char *line = NULL;
    /*char *dict = NULL; */
    int i=0,g=0;
    signal(SIGPIPE,handlePipe);
    if((fd=fopen(argv[1],"r"))<0){
        fprintf(stderr,"Error opening file %s for reading: %s\n",argv[1],strerror(errno));
        return -1;
    }
    while((g=getline(&dic[i],&dWrdS,fd))!=-1){
        if (g>1){
            if(dic[i][g-2]=='\r'){
                dic[i][g-2]='\0';
            }
            else{
                dic[i][g-1]='\0';
            }
        }
        else{
            dic[i][g-1]='\0';
        }
        i++;
    }
    fprintf(stdout,"Accepted %i words\n",i);
    while ((g=getline(&line,&dWrdS,stdin))!=-1){
        if(line!=NULL){
            if (g>1){
                if(line[g-2]=='\r'){
                    line[g-2]='\0';
                }
                else{
                    line[g-1]='\0';
                }
            }
            else{
                line[g-1]='\0';
            }

            for (int j=0; j<i; j++){
                if(strcmp(dic[j],line)==0){
                    fprintf(stdout,"%s\n",line);
                    m++; 
                    break;
                }
            }
        }
    }
   
    fprintf(stderr, "Matched %i words\n",m );
    
    fclose(fd);
}

