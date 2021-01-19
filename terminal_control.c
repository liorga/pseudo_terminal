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
    int flag = 0;
    struct termios terminal;
    int res = 0;
    

    if (strcmp(argv[1],"echo") == 0)
    {
        if (strcmp(argv[2],"on") == 0)
        {
            flag = 1;
        }

        if (strcmp(argv[2],"off") == 0)
        {
            flag = 0;
        }
        
        res = ctl_echo(STDIN_FILENO,flag);
        
        
    }else if (strcmp(argv[1],"eof") == 0)
    {
        if (strcmp(argv[2],"on") == 0)
        {
            flag = 1;
        }

        if (strcmp(argv[2],"off") == 0)
        {
            flag = 0;
        }
        
        res = ctl_eof(STDIN_FILENO,flag);
    }

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

