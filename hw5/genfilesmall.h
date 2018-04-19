void genSFile(char* filename){ 
    int bufsize=70; 
    char buf[bufsize];
    int filed; 
    int wresult; //write result
    filed = open(filename,O_WRONLY|O_CREAT|O_TRUNC,0666);// open for writing
    if(filed==-1){
        fprintf(stderr,"error attempting to open file %s for O_WRONLY|O_CREAT|O_TRUNC: %s\n",filename,strerror(errno));
        exit (-1);
    }
    for(int i =0; i<bufsize;i++){//create buffer array to size of the file 
        buf[i]='a';
    }
    wresult = write(filed,buf, bufsize);//write buffer to the newly created or truncated file
    if(wresult==-1){// error on -1 
        fprintf(stderr,"error attempting to write to a file %s for O_WRONLY|O_CREAT|O_TRUNC: %s\n",filename,strerror(errno));
        exit (-1);
    }
    else if (wresult<bufsize){//incomplete write
        fprintf(stderr,"error attempting to write %d bytes to file %s, only %d written \n",7000,filename,wresult);
        exit (-1);
    }
    close(filed);
return;
}

