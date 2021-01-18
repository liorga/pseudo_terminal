#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termio.h>
#include <stdlib.h>

int ctl_echo( int fd, int flag );
int ctl_eof( int fd, int flag );


int main(int argc, char const *argv[])
{
    int flag;
    struct termios teminal;
    int fd;


    

    return 0;
}

int ctl_echo( int fd, int flag ){

    struct termios term;

    if (!isatty(fd))
    {
        return EXIT_FAILURE;
    }else{
        if(tcgetattr(fd,&term) < 0){
            perror("tcgetattr failed");
            return EXIT_FAILURE;
        }

        if (!flag){
            term.c_lflag &= ~ECHO;
        }else{
            term.c_lflag |= ECHO;
        }
        
        if (tcsetattr(fd,TCSAFLUSH,&term) < 0){
            perror("tcsetattr failed");
            return EXIT_FAILURE;
        }else
        {
            return EXIT_SUCCESS;
        }
        
        
    }
    
    


}
