#include <stdio.h>
#include <string.h>

int readIn(char** myArg, int* tokNum){ 
    char b;
    int bufIndex = 0;
    int iArg=0; 
    char* token; 
    char delim[]= " \t"; 
    char buf[4096];
    while ((b=getc(stdin))!='\n'){//get the chars until newline or EOF or comment
        if(b==EOF){
            return 1;
        }
        else if(b=='#'){//loop to the end of the line but dont return 0 because a comment is a valid operation/thing to do
            while((b=getc(stdin))!='\n'){}
            break; 
        }
        else{
            buf[bufIndex]=b;
            bufIndex++;
        }
    }
    buf[bufIndex]='\0';//give a null terminator so that we dont have issues
    fprintf(stderr, "%s",buf);// fix: this is a debug line remove it 
    token = strtok(buf,delim);//gets first token
    while(token){
        myArg[iArg]=token;
        iArg++;
        token = strtok(NULL,delim);//continues getting new tokens
    }
    myArg[iArg]=NULL;
    *tokNum=iArg;
    
    fprintf(stderr, "lol|%d|%d|",iArg,*tokNum);// fix: this is a debug line remove it 
    return 0;  
}



void rmRedirectSym(int rem,int j,int l, char* old, char*nnew){
    int k=0;
    while(old[k+rem]!=NULL){
        nnew[l][k]=old[j][k+rem];
        k++;
    }

}





int main(int argc, const char* argv[]){
    int x =0;//exit status var for exiting while loop
    int i=0;//split line index var
    int tNum=0;//
    //int spInd=0;//spline index used for removal of unnessesary checks for flags and or args and or redirects
    char* spline[2048];//Split Line Array of pointers to chars
    //char* args[2045];
    char* redir[5];//[<,>,2>,>>,2>>]
    int  r0=0,r1=0,r2=0;    
    while (x==0){
        //read from std in 
        x=readIn(spline,&tNum);//line is returned so is spline which is basically argv and tNum is argc fix: remove the 'line' argument cuz it's unnessesary
        fprintf(stderr, "@%d\n",tNum);// fix: this is a debug line remove it 
        //initiallize everything  
        i=0;
        r0=0;
        r1=0;
        r2=0;
        for (int l=0;l<5;l++){
            redir[l]=NULL;
        }


        //spInd is the location of the last noted command/thing is command so we skip it lets look for flags
      
        while(spline[i]!=NULL && i<1024){
            fprintf(stderr," ");
            //fprintf(stderr,"|%lu|",strlen(spline[i]));
            fprintf(stderr,"%s+",spline[i]);
            i++; 
        }
        fprintf(stderr,"\n");
        
        
        for(int j=0; j<tNum;j++){//fix: need to check if we read three redirects in which case we should fail

            if( r0>1||r1>1||r2>1){
                fprintf(stderr,"Error, too many redirections in previous line " );
                break;
            }
            if(strlen(spline[j])>=3){
                if( spline[j][0]==  '<'){//red0
                    fprintf(stderr, "< -%d\n",j);// fix: this is a debug line remove it
                    rmRedirectSym(1,j,0,spline[j],redir[0]); 
                    r0++;
                    //redirect stdin     
                }
                else if( spline[j][0]=='>'){//red3
                    if (strlen(spline[j])>=4 && spline[j][1]=='>'){
                        fprintf(stderr, ">> -%d\n",j);// fix: this is a debug line remove it
                        //>> redirect stdout append flag
                        rmRedirectSym(2,j,3,&spline,&redir[3]); 
                        r1++;
                    }
                    else if(strlen(spline[j])>=4 ){//red1
                        fprintf(stderr, "> -%d\n",j);// fix: this is a debug line remove it
                        //> redirect stdout for truncate mode
                        rmRedirectSym(1,j,1,&spline[j],&redir[1]); 
                        r1++;
                    }
                            
                }
                else if( spline[j][0]=='2'){
                    if ( strlen(spline[j])>=4 && spline[j][1]=='>'){
                        if ( strlen(spline[j])>=5 && spline[j][2]=='>'){
                            // 2>> stderr append
                            rmRedirectSym(3,j,4,&spline[j],&redir[4]); 
                            r2++;
                        }
                        else{//red2
                            //stderr in trunc mode 
                            //this is the 2> since it has 2> but failed the third>
                            rmRedirectSym(2,j,2,&spline[j],&redir[2]); 
                            r2++;
                        } 
                    }
                }
            }
        }
        i=0; 
        fprintf(stderr,"\n");
        fprintf(stderr,"\n");
        //ignore this shit
        while(i<5){
            fprintf(stderr,"%d",i);
            //fprintf(stderr,"|%lu|",strlen(spline[i]));
            fprintf(stderr,"%s--",redir[i]);
            i++; 
        }
        fprintf(stderr,"\n");
        /*while(spline[i]!=NULL && i<1024){*/
            /*fprintf(stderr," ");*/
            /*//fprintf(stderr,"|%lu|",strlen(spline[i]));*/
            /*fprintf(stderr,"%s+",spline[i]);*/
            /*i++; */
        /*}*/
        /*fprintf(stderr,"\n");*/




        //fork and exec portion
   }    
    return 0;  
}
