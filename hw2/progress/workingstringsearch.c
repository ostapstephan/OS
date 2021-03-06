#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>


int same(char *target, char *fdTest){
   // int fdc = open(fdTest,O_RDONLY); //fd of check file
    //printf("%s",fdTest);
}


int isDot(char const *name){//is it a dot or dot dot 
    if (strcmp(name,".")==0)
        return 0;
    if (strcmp(name,"..")==0)
        return 0;
    return 1;
}


int search(char *path, char *target){ 
    DIR *dp;
    struct dirent *ds;
    struct stat elemInfo; 

    char pathn[1024]; 

    dp = opendir(path); // this has to get reasonable path that is ./joey or ./joey/ostap
    
   // fprintf(stderr,"Start of search()\n");
     
    //chdir(path); //we need a path= path +/  + path kinda thing
    //fprintf(stderr,"you are in %s \n",path); 
    while (ds=readdir(dp)){ 
        //fprintf(stderr,"%i \n",ds->d_type);
        if(isDot(ds->d_name)==0){
            continue;
        }
        
        snprintf(pathn,sizeof(pathn),"%s/%s",path,ds->d_name); 
        //fprintf(stderr,"--%s \n",pathn);
        
        lstat(pathn,&elemInfo); 
        switch(elemInfo.st_mode & S_IFMT){
            case S_IFREG:
                fprintf(stderr,"is a regular file: %s\n", ds->d_name);
               // same(target,pathn);

                //reg file
                break;
            case S_IFDIR:
                fprintf(stderr,"passes the dot and dir check: %s%i\n\n",ds->d_name,(S_ISDIR(elemInfo.st_mode)));
                search(pathn,target);
                //Directorly
                break;
            case S_IFLNK:
                fprintf(stdout,"this is a symlink"); 
                //symlink file
                break;
            default:
                break;
        }       
 
        /*if ((S_ISDIR(elemInfo.st_mode))){*/
            /*fprintf(stderr,"passes the dot and dir check: %s\n\n",ds->d_name);*/
            /*search(ds->d_name,target);*/
            /*}*/
        /*else fprintf(stderr,"%s\n", ds->d_name);*/
    }
    //chdir("..");
    closedir(dp);
    return 0; 
}

int main(int argc, char **argv){ 
   // char c[]= ".."; 
    
    if(argc !=2){
       fprintf(stderr,"Wrong amount of args\n");  
    }
    search(argv[1],argv[0]);
    //printf("%i",(isDot(c))); 
}
