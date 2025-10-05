#if 1

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

