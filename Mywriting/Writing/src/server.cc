#if 1

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


