#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>

int appendFile(int fdr,int fdw, int bufsize, char *fnr, char *fnw){
	char buf[bufsize+1];
	int bRead = 0;
	int bwritten = 0;
	while((bRead = read(fdr, buf, bufsize)) > 0){
		if((bwritten=write(fdw, buf, bRead))!= bRead){
			if(bwritten==-1){
				fprintf(stderr,"Error, failed write on file: %s\n While reading from %s\n %s\n", fnw, fnr, strerror(errno));
				return -1;
			}			
			fprintf(stderr,"Error, partial write on file: %s\n while reading from %s \n%s\n", fnw,fnr, strerror(errno));
			return -1;
		}
	}
	if(bRead==-1){
		fprintf(stderr,"Error, unable to read file %s\n %s\n", fnr, strerror(errno));
		return -1;
	}
}
int main(int argc, char **argv){
	int bufBytes = 1024;
	int count = argc;
	char c = 0;
	int fd = STDIN_FILENO; //default to stdin
	int out_fd = STDOUT_FILENO; //standard output is default
	char *fn;
	char *outputFN = NULL;  
	clock_t begin = clock();

	while ((c = getopt (argc, argv, "b:o:")) != -1){
		switch (c){
			case 'b':
				bufBytes = atoi(optarg);
				break;
			case 'o':
				outputFN = optarg; //
				break;
			case '?':
				if (optopt == 'b'|| optopt == 'o'){
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
					return -1;
				} 
				else if (isprint (optopt)){
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
					return -1;
				}
				else{
					fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
					return -1;
				}
				return 1;
			default:
				fprintf (stderr,"Usage: minicat [-o outpufile] [-b ###] infile1 [...infile2...]");
				return -1;
		}
	}
	if (outputFN != NULL){
		if ((out_fd = open (outputFN, O_WRONLY|O_CREAT|O_TRUNC, 0666))== -1){
			fprintf (stderr, "Error opening %s for writing: \n%s\n ", outputFN , strerror(errno));
			return -1;
		}
	}
	if (optind != argc){
		for (int i = 0; i + optind < argc; i++){
			if ((strcmp(argv[i+ optind],"-"))!=0){//std input read if theyre the same	
				if ((fd = open(argv[i+ optind],O_RDONLY,0666)) ==-1){
					fprintf (stderr,"Error Cant open file %s for reading : \n%s\n",argv[i+ optind], strerror(errno));	
					return -1;
				}
				if((appendFile(fd,out_fd,bufBytes,argv[i+ optind],outputFN))==-1){
					return -1;
				}
				if (close(fd)!=0){	
					fprintf(stderr, "Error closing file %s: \n%s\n ", argv[i+ optind], strerror(errno));	
					return -1;
				}
			}
			else{
				fn = "stdin";
				fd = STDIN_FILENO;
				if ((appendFile(fd, out_fd, bufBytes, fn, outputFN))==-1){
					return -1;
				}
			}		
		}
	}
	else{
		fn = "stdin";
		fd = STDIN_FILENO;
		if((appendFile(fd, out_fd, bufBytes, fn, outputFN))==-1){
			return -1;
		}
	}	
	clock_t end = clock();
	double time_spent = ((double)(end - begin)) / CLOCKS_PER_SEC;
	printf("%f\n",time_spent);
	return 0;
}
