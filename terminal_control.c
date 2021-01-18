#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termio.h>
#include <stdlib.h>
#include <string.h>

int ctl_echo( int fd, int flag );
int ctl_eof( int fd, int flag );


int main(int argc, char const *argv[])
{
    int flag;
    struct termios terminal;
    int fd,res;
    
    if(tcgetattr(STDIN_FILENO,&terminal) < 0){
        perror("tcgetattr2 failed");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1],"echo on"))
    {
        flag = 1;
    }else if (strcmp(argv[1],"echo off"))
    {
        flag = 0;
    }


    res = ctl_echo(STDIN_FILENO,flag);

    //res = ctl_eof(STDIN_FILENO,flag);
    
    

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
        
        if (tcsetattr(fd,TCSAFLUSH,&term) < 0){
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
        
        if (tcsetattr(fd,TCSAFLUSH,&term) < 0){
            perror("tcsetattr failed");
            return EXIT_FAILURE;
        }
    }
    
    return EXIT_SUCCESS;
}

