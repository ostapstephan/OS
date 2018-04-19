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
    //fprintf(stderr, "%s",buf);// fix: this is a debug line remove it 
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

int main(int argc, const char* argv[]){
    int x =0;//exit status var for exiting while loop
    int i=0;//split line index var
    int tNum=0;//
    int splSize =2048;
    //int spInd=0;//spline index used for removal of unnessesary checks for flags and or args and or redirects
    char* spline[splSize];//Split Line Array of pointers to chars
    char* args[splSize-3];
    char* redir[5];//[<,>,2>,>>,2>>]
    int r[3]={0,0,0}; 
    int k[3]={-1,-1,-1};
    while (x==0){
        //read from std in 
        x=readIn(spline,&tNum);//line is returned so is spline which is basically argv and tNum is argc fix: remove the 'line' argument cuz it's unnessesary
        fprintf(stderr, "@%d\n",tNum);// fix: this is a debug line remove it 
        //initiallize everything  
        i=0;
        r={0,0,0};
        k={-1,-1,-1};        
        for (int l=0;l<5;l++){
            
            fprintf(stderr,"x");
            redir[l]=" ";

        }

        //spInd is the location of the last noted command/thing is command so we skip it lets look for flags
        for(int j=0; j<tNum;j++){
            fprintf(stderr,"Start\n");
            if(strlen(spline[j])>=3){
                fprintf(stderr,"min3\n");
                if( spline[j][0]==  '<'){
                    redir[0]=spline[j];
                    k=0;
                    while(spline[j][k+1]!='\0'){
                        redir[0][k]=spline[j][k+1];
                        k++;
                    }
                    redir[0][k]='\0';
                    r[0]++;
                    k[0]=j;
                    //redirect stdin     
                }
                else if( spline[j][0]=='>'){
                    if (strlen(spline[j])>=4 && spline[j][1]=='>'){
                        //>> redirect stdout append flag
                        redir[2]=spline[j];
                        k=0;
                        while(spline[j][k+2]!='\0'){
                            redir[2][k]=spline[j][k+2];
                            k++;
                        } 
                        redir[2][k]='\0';
                        r[1]++;
                        k[1]=j;
                    }
                    else{
                        //> redirect stdout for truncate mode
                        redir[1]=spline[j];
                        k=0;
                        while(spline[j][k+1]!='\0'){
                            redir[1][k]=spline[j][k+1];
                            k++;
                        } 
                        redir[1][k]='\0';
                        r[1]++;
                        k[1]=j;
                    }
                            
                }
                else if( spline[j][0]=='2'){
                    if ( strlen(spline[j])>=4 && spline[j][1]=='>'){
                        if ( strlen(spline[j])>=5 && spline[j][2]=='>'){
                            // 2>> stderr append
                                   
                            redir[4]=spline[j];
                            k=0;
                            while(spline[j][k+3]!='\0'){
                                redir[4][k]=spline[j][k+3];
                                k++;
                            } 
                            
                            redir[4][k]='\0';
                            r[2]++;
                            k[2]=j;
                        }
                        else{
                            //stderr in trunc mode 
                            //this is the 2> since it has 2> but failed the third>
                            redir[3]=spline[j];
                            k=0;
                            while(spline[j][k+2]!='\0'){
                                redir[3][k]=spline[j][k+2];
                                k++;
                            } 
                            redir[3][k]='\0';
                            r[2]++;
                            k[2]=j;
                        } 
                    }
                }
                i=0;
                if( r0>1||r1>1||r2>1){
                    fprintf(stderr,"Error, too many redirections in previous line " );
                    break;
                }
            }
        }
        i=0; 
        fprintf(stderr,"\nE\n");
        //ignore this shit
        
        while(i<5){
            fprintf(stderr," ");
            //fprintf(stderr,"|%lu|",strlen(spline[i]));
            fprintf(stderr,"%s%i",redir[i],i);
            i++; 
        }
        fprintf(stderr,"\n\n\n");

        //still ignore
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
