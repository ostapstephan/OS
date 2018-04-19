#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

void genRan(){
    int nc=10;//nc max length of the word
    char rWord[nc+1];//allocate space for the word
    char let[26]={"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};//alphabet
    int j=0;//loop varible for the word
    int r=-1;//random number set to negative number to prevent it from being read into the array 
    int c;
    r=rand();
    c=r%(nc-2)+3;
    for(j=0;j<c;j++){
        r = rand()%26;
        rWord[j]=let[r%26]; 
    }
    rWord[j]='\0';
    fprintf(stdout,"%s\n",rWord);
}

int main(int argc, char* argv[]){
    int aI =0;//return Val for atoi
    srand(time(0));
    if(argc>1){
        if( (aI=atoi(argv[1]))>0){
            for(int l=0;l<aI;l++){
                genRan();
            }
        }
        else{
             while(1){ 
               genRan();
            }
        }
    }
    else{
        while(1){ 
            genRan();
        }
    }
    return 0;
}

