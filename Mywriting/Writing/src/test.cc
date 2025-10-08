#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

int main()
{
    int pipefd1[2];
    int pipefd2[2];

    // creat pipe
    if (pipe(pipefd1) == -1 || pipe(pipefd2)==-1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    int pid=fork();
    if(pid!=0)
    {
        char buf1[64]={0};
        char buf2[64]={0};
        //in parent process
        //关闭pipefd1的读端
        close(pipefd1[0]);
        //关闭pipefd2的写端
        close(pipefd2[1]);

        //给子进程写一个字符
        int w_bytes=write(pipefd1[1],&buf1,1);
        if(w_bytes<=0)
        {
            perror("write failed");
            exit(EXIT_FAILURE);
        }

        //读取子进程写给父进程的字符
        int r_bytes=read(pipefd2[0],&buf2,1);
        if(r_bytes>0)
        {
            printf("<%d>:received pong\n",getpid());
        }
    }else
    {
        char buf1[64]={0};
        char buf2[64]={0};
        //in child procesws
        //关闭pipefd1的写端
        close(pipefd1[1]);
        //关闭pipefd2的读端
        close(pipefd2[0]);

        int r_bytes=read(pipefd1[0],&buf1,1);
        if(r_bytes>0)
        {
            printf("<%d>:received ping\n",getpid());
        }
        int w_bytes=write(pipefd2[1],&buf2,1);
        if(w_bytes<=0)
        {
            perror("write failed");
            exit(EXIT_FAILURE);
        }
    }

    
    return 0;
}