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

    if (!isatty(fd))
    {
        perror("given fd is not a terminal!");
        exit(EXIT_FAILURE);
    }
    
    
}
