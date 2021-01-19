#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termio.h>
#include <stdlib.h>
#include <string.h>

int dbl_copy( int f1, int t1, int f2, int t2 );

int main(int argc, char const *argv[])
{
    
    pid_t pid;
    int pipid[2];
    int res;

    if(pipe(pipid) == -1){
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid < 0)
    {
        return 2;
    }else
    {
        //chiled proccess
        
    }
    
    




    return 0;
}
