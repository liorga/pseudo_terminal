#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termio.h>
#include <stdlib.h>
#include <string.h>
//#define _XOPEN_SOURCE
#define BUFF_SIZE 1024


int ctl_echo( int fd, int flag );
int ctl_eof( int fd, int flag );
ssize_t dbl_copy( int f1, int t1, int f2, int t2 );



int main(int argc, char const *argv[])
{
    int fds,fdm;
    pid_t pid;
    
    //char* slavename = NULL;
    //create master
    fdm = posix_openpt(O_RDWR);
    char* slavename = (char*)malloc(1024*sizeof(char));
   


    pid = fork();
    if (pid == 0)
    {
        //child proccess
        
        //give slave permissions
        if (grantpt(fdm) == -1)
        {
            perror("grantpt failed");
            exit(EXIT_FAILURE);
        }
        //unlock
        if (unlockpt(fdm) == -1)
        {
            perror("unlockpt failed");
            exit(EXIT_FAILURE);
        }

        //get slave path name
        if (( ptsname_r(fdm,slavename,BUFF_SIZE)) == -1)
        {
            perror("ptsname failed");
            exit(EXIT_FAILURE);
        }
        //open slave using path
        fds = open(slavename,O_RDWR);
		if (fds == -1) {
			perror("open");
			exit(EXIT_FAILURE);
		}

        //close master
        close(fdm);

        //rediract 0,1,2 to fds
        dup2(fds,STDIN_FILENO);
        dup2(fds,STDOUT_FILENO);
        dup2(fds,STDERR_FILENO);
        //close fds
        close(fds);

        //call cat using exec
        execlp("cat","cat",NULL);

        return 0;
    }
    
    //perent
    while(1){
        dbl_copy(STDIN_FILENO,fdm,fdm,STDOUT_FILENO);
    }

    free(slavename);
    return 0;
}



int ctl_echo( int fd, int flag ){

    struct termios term;

    if (!isatty(fd)){
        return EXIT_FAILURE;
    }
    else{

        if(tcgetattr(fd,&term) < 0){
            perror("tcgetattr3 failed");
            return EXIT_FAILURE;
        }

        if (!flag){
            term.c_lflag &= ~ECHO;
        }
        else{
            term.c_lflag |= ECHO;
        }
        
        if (tcsetattr(fd,TCSADRAIN,&term) < 0){
            perror("tcsetattr failed");
            return EXIT_FAILURE;
        }
    }
    
    return EXIT_SUCCESS;
}

int ctl_eof( int fd, int flag ){

    struct termios term;

    if (!isatty(fd)){
        return EXIT_FAILURE;
    }
    else{

        if(tcgetattr(fd,&term) < 0){
            perror("tcgetattr4 failed");
            return EXIT_FAILURE;
        }

        if (!flag){
            term.c_cc[VEOF] = _POSIX_VDISABLE;
        }
        else{
            term.c_cc[VEOF] = 4;
        }
        
        if (tcsetattr(fd,TCSADRAIN,&term) < 0){
            perror("tcsetattr failed");
            return EXIT_FAILURE;
        }
    }
    
    return EXIT_SUCCESS;
}

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

			//printf("\t%d: <message from fifo is>:  %s\n",STDOUT_FILENO,buffer);
		}

	FD_CLR(f1,&temp_fds);
	FD_CLR(f2,&temp_fds);
	FD_ZERO(&temp_fds);
	free(buffer);

    return total_bytes;
}
