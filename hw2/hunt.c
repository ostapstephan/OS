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
#include <stdio.h>

int same(char *target, char *Test,int sLink){
    struct stat TarStat; //target
    struct stat TestStat; 
    FILE *fdTar, *fdtest;
    int tar,tes,perm;
    char *HS[2] ={"Hard","Sym"};
    char *hs2[2]={"","Symlink"}; 
    char *yn[2]={"no","yes"};

    if (stat(target,&TarStat)==-1){
        fprintf(stderr,"Error encountered when attempting to stat target file  %s \n%s ",target, strerror(errno)); 
        exit(-1); 
    }

    if ( stat(Test,&TestStat)==-1){
        fprintf(stderr,"Error encountered when attempting to stat test file  %s \n%s ",Test, strerror(errno)); 
        exit(-1); 
    }
   
    if((TestStat.st_ino)==(TarStat.st_ino)){
        fprintf(stdout,"%s \tIs a %slink to target nLinks = %i \n",Test,HS[sLink],(int)TestStat.st_nlink);
        if (sLink==0){
            perm=(TestStat.st_mode&S_IROTH)==S_IROTH; 
            fprintf(stdout,"Other can read: %s \n\n", yn[perm]);
        }
        return 2;
    }

    if ( (TarStat.st_size==TestStat.st_size)){
        if((fdTar = fopen(target,"r"))== NULL){
            fprintf(stderr,"Error opening Target file  %s \n%s ",target, strerror(errno)); 
            exit(-1); 
        }
        if((fdtest = fopen(Test,"r"))== NULL){
            fprintf(stderr,"Error opening Test file  %s \n%s ",Test, strerror(errno)); 
            exit(-1); 
        }
        tar = getc(fdTar);
        tes = getc(fdtest);
        
        while ((tar!=EOF)&&(tar!=EOF)&&(tar==tes)){
            tar = getc(fdTar);
            tes = getc(fdtest);
        } 
   
       if (tar==tes){
           if ((int)TestStat.st_nlink>1){
                fprintf(stdout,"%s \t %s Matches Target File nLinks = %i \n",Test,hs2[sLink],(int)TestStat.st_nlink);
                if (sLink==0){
                    perm=(TestStat.st_mode &S_IROTH)==S_IROTH; 
                    fprintf(stdout,"Other can read: %s \n\n", yn[perm]);
                }
           }
            else{
                fprintf(stdout,"%s \t %s Matches Target File \n",Test,hs2[sLink]);
                if (sLink==0){
                    perm=(TestStat.st_mode &S_IROTH)==S_IROTH; 
                    fprintf(stdout,"Other can read: %s \n\n", yn[perm]);
                }
            } 

        return 1;

       }
       else
           return 0;
        
        fclose(fdTar);
        fclose(fdtest);
   } 
    return 0;
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
    char sym[1024];
    int sEnd=0;

    if((dp = opendir(path))==NULL){ // this has to get reasonable path that is ./joey or ./joey/ostap
        fprintf(stderr,"Error opening directory %s \n%s ",path, strerror(errno)); 
        return -1; 
    }
    
    while ((ds=readdir(dp))){ 
        //fprintf(stderr,"%i \n",ds->d_type);
        if(isDot(ds->d_name)==0){
            continue;
        }
        
        snprintf(pathn,sizeof(pathn),"%s/%s",path,ds->d_name); 
        //fprintf(stderr,"--%s \n",pathn);
        
        if ( lstat(pathn,&elemInfo)==-1){
            fprintf(stderr,"Error encountered when attempting to stat test file  %s \n%s ", pathn, strerror(errno)); 
            exit(-1); 
        }

        
        lstat(pathn,&elemInfo); 
        switch(elemInfo.st_mode & S_IFMT){
            case S_IFREG:
                same(target,pathn,0);
                //reg file
                break;
            case S_IFDIR:
                search(pathn,target);
                //Directory
                break;
            case S_IFLNK:
                if( same(target,pathn,1)>0){//checks sameness and prints it 
                    sEnd=readlink(pathn,sym,1024);
                    sym[sEnd]='\0'; 
                    fprintf(stdout,"Symlink encountered points to %s \n \n",sym); 
                }
                //symlink file
                break;
            default:
                fprintf(stdout,"%s -Unknown type not file symilink or directory",pathn); 
                
                break;
        }       
 
    }
    closedir(dp);
    return 0; 
}

int main(int argc, char **argv){ 
    
    if(argc !=3){
       fprintf(stderr,"Wrong amount of args\n");  
        exit (-1); 
    }
    search(argv[1],argv[2]);
}
