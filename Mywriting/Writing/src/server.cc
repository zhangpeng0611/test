#if 0

#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
using namespace std;

const int PORT=8080;
char buf[1024]={0};
const char* msg="hello,client";

int main(){

    //创建通信用的socket
    int sock=socket(AF_INET,SOCK_STREAM,0);
    
    //给sock绑定ip地址和端口号
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(PORT);
    addr.sin_addr.s_addr=INADDR_ANY;
    bind(sock,(sockaddr*)&addr,sizeof(addr));

    //监听sock
    listen(sock,3);
    cout<<"服务器正在监听:"<<PORT<<"端口"<<endl;

    //accept接收客户端的连接
    int addrlen=sizeof(addr);
    int connfd=accept(sock,(sockaddr*)&addr,(socklen_t*)&addrlen);

    //接收客户端传来的数据
    read(connfd,(void*)buf,1024);
    cout<<"接收到客户端传来的消息为："<<buf<<endl;

    //给客户端回传消息
    write(connfd,msg,strlen(msg));
    cout<<"给客户端回传消息:"<<msg<<endl;
    
    close(sock);
    close(connfd);
    return 0;
}

#endif


#if 1

// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BACKLOG 5 // 最大连接等待队列长度

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // 1. 创建套接字 (IPv4, TCP, 0)
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 设置套接字选项，允许端口重用
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // 监听所有网络接口的 IP
    address.sin_port = htons(PORT);       // 将端口号转换为网络字节序

    // 2. 绑定套接字到 IP 和端口
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 3. 开始监听
    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d...\n", PORT);

    // 4. 接受客户端连接 (这会阻塞直到有连接)
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    
    printf("Client connected! Waiting for 30 seconds before closing...\n");

    // 5. 等待 30 秒，给我们足够的时间去抓包
    sleep(30);

    // 6. 关闭连接
    close(new_socket);
    printf("Connection closed.\n");
    
    return 0;
}

#endif
