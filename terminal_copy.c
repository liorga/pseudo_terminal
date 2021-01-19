#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termio.h>
#include <stdlib.h>
#include <string.h>
#define BUFF_SIZE 1024

ssize_t dbl_copy( int f1, int t1, int f2, int t2 ){
    size_t bufsize = BUFF_SIZE;
	fd_set readers_fds,temp_fds;
	ssize_t total_bytes = 0,bytes = 0;
	char* buffer = (char*)malloc(BUFF_SIZE*sizeof(char));	
	FD_ZERO(&readers_fds);
	FD_SET(f1,&readers_fds);
	FD_SET(f2,&readers_fds);
	temp_fds = readers_fds;
	if (select(FD_SETSIZE,&temp_fds,NULL,NULL,NULL) < 0){
		perror("select");
		exit(EXIT_FAILURE);
	}
	if (FD_ISSET(f1,&temp_fds)){
		if ((bytes = read(f1, buffer, bufsize)) == -1) {
			perror("read f1");
			exit(EXIT_FAILURE);
		}
		if(strncmp(buffer,"quit",strlen("quit")) == 0){
			return total_bytes;
		}else{
            if(write(t1, buffer, strlen(buffer)) == -1) {
                perror("write t1");
                return(EXIT_FAILURE);
            }          
        }  
	}
    if (FD_ISSET(f2,&temp_fds)){
		if ((bytes = read(f2, buffer, bufsize)) == -1) {
			perror("read f2");
			exit(EXIT_FAILURE);
		}
        total_bytes += bytes;     
		if(strncmp(buffer,"quit",strlen("quit")) == 0){
			return total_bytes;
		}else{
            if(write(t2, buffer, strlen(buffer)) == -1) {
                perror("write t2");
                return(EXIT_FAILURE);
             }       
        }  
	}
    FD_CLR(f1,&temp_fds);
    FD_CLR(f2,&temp_fds);
    FD_ZERO(&temp_fds);
    free(buffer);
    return total_bytes;
}