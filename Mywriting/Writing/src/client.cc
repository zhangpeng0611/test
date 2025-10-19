#if 0

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

using namespace std;

const int PORT=8080;
const char* SERVER_IP="127.0.0.1";
const char* msg="hello server";
char buf[1024]={0};

int main(){

    //创建套接字
    int sock=socket(AF_INET,SOCK_STREAM,0);

    //连接服务器
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(PORT);
    inet_pton(AF_INET,SERVER_IP,&addr.sin_addr);
    cout<<"连接服务器:"<<SERVER_IP<<":"<<PORT<<endl;
    connect(sock,(sockaddr*)&addr,sizeof(addr));

    //给服务端发送消息
    write(sock,msg,strlen(msg));
    cout<<"给服务端发送消息:"<<msg<<endl;

    //接收服务端发送过来的消息
    read(sock,buf,1024);
    cout<<"接收到服务端发来的信息为:"<<buf<<endl;

    close(sock);
    return 0;
}

#endif

#if 1

// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1" // 连接本地服务器

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;

    // 1. 创建套接字
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 将 IPv4 地址从文本形式转换为二进制形式
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0)
    {
        perror("invalid address");
        exit(EXIT_FAILURE);
    }

    // 2. 连接到服务器
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server successfully!\n");

    // 3. 等待 25 秒，然后关闭连接
    sleep(25);
    printf("Client closing connection.\n");

    // 4. 关闭连接
    close(sock);

    return 0;
}

#endif

