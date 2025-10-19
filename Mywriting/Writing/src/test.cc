#if 0

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

#endif

#if 0

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define ZERO 0
#define ONE 1
#define SIZE 35

void primes(int received_fd)
{
    int num[SIZE];
    int fd[2];
    pipe(fd);

    // 父进程传过来的数据进行接收
    if(read(received_fd, num, SIZE)==0)
    {
        return;
    }

    // 对传过来的数组中的数据进行过滤

    int val = 0; // 存储每次新接收父进程传来的数组中的第一个数
    for (int i = 0; i < SIZE; ++i)
    {
        if (num[i] == ONE)
        {
            val = i;
            num[i] = ZERO;
            break;
        }
    }
    if (val == 0)
        return;
    printf("prime %d\n", val);

    for (int i = 0; i < SIZE; i++)
    {
        if (i % val == 0)
        {
            num[i] = ZERO;
        }
    }

    int pid = fork();
    if (pid > 0)
    {
        close(fd[0]);
        write(fd[1], num, SIZE);
        close(fd[1]);
        wait(0);
    }
    else
    {
        close(fd[1]);
        primes(fd[0]);
        close(fd[0]);
    }
}

int main()
{
    // 创建管道
    int fd[2];
    pipe(fd);

    int num[SIZE];
    num[0] = ZERO;
    num[1] = ZERO;

    for (int i = 2; i < SIZE; ++i)
    {
        num[i] = ONE;
    }

    int pid = fork();
    if (pid > 0)
    {
        // in parent process
        close(fd[0]);  //关闭不适用的读端
        write(fd[1], num, SIZE);
        close(fd[1]);  //写完关闭写端，父进程写完数据必须关闭写端，这样子进程read的时候才会读到文件结束符，然后才不会阻塞
        wait(0);
    }
    else if (pid == 0)
    {
        // in child process
        close(fd[1]);  //关闭写端
        primes(fd[0]);
        close(fd[0]);  //读完关闭读端
    }

    exit(0);
}

#endif

#if 1

#include <bits/stdc++.h>
#include <memory>
using namespace std;

class Child;

class Parent
{
public:
    Parent()
    {
        cout << "Parent 构造函数" << endl;
    }
    ~Parent()
    {
        cout << "Parent 析构函数" << endl;
    }

    shared_ptr<Child> child_;
};

class Child
{
public:
    Child()
    {
        cout << "Child 构造函数" << endl;
    }
    ~Child()
    {
        cout << "Child 析构函数" << endl;
    }

    shared_ptr<Parent> parent_;
};

int main()
{
    shared_ptr<Parent> ptr1 = make_shared<Parent>();
    shared_ptr<Child> ptr2 = make_shared<Child>();
    ptr1->child_ = ptr2;
    ptr2->parent_ = ptr1;
    return 0;
}

#endif