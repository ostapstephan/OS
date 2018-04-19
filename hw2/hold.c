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
    printf("%s",fdTest);
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

    dp = opendir(path);
    fprintf(stderr,"Start of search()\n");
    chdir(path);   
    while (ds=readdir(dp)){
        lstat(ds->d_name,&elemInfo);
        //fprintf(stderr,"--%s \n",ds->d_name);
        //fprintf(stderr,"%i \n",ds->d_type);
        if(isDot(ds->d_name)==0){
            continue;
        }
        
        if ((S_ISDIR(elemInfo.st_mode))){
            fprintf(stderr,"passes the dot and dir check: %s\n\n",ds->d_name);
            search(ds->d_name,target);
            }
        else fprintf(stderr,"%s/n", ds->d_name);
    }
    chdir("..");
    closedir(dp);
    return 0; 


int main(int argc, char **argv){ 
   // char c[]= ".."; 
    
    if(argc !=2){
       fprintf(stderr,"Wrong amount of args\n");  
    }
    search(argv[1],argv[0]);
    //printf("%i",(isDot(c))); 


}





	
        switch(elemInfo.st_mode & S_IFMT){
            case S_IFREG:
                //reg file
                printf("%s",ds->d_name);
            case S_IFDIR:
                fprintf(stderr, "|%s", ds->d_name);                
                fprintf(stderr,"d\n");
                //Directory
                search(ds->d_name,target);
            case S_IFLNK:
                //symlink file
                ;
            default:
                ;
        }       

