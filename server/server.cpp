//
// Created by ChenXuzheng on 2021/10/5.
//

#include "../config.h"
#include "data.h"
#include "receive.h"

struct serverStatus* ss= nullptr;
int main(){
    //监听的套接字
    int listenFd,connFd;
    pthread_t recvTid,sendTid;
    //地址的套接字
    struct sockaddr_in  clientAddr;
    //用于传参的常数
    socklen_t clientLength=sizeof(clientAddr);

    if((listenFd= socket(AF_INET,SOCK_STREAM,0))==-1){
        perror("socket init error, exit!");
        exit(1);
    }
    ss=new serverStatus(SERVERNAME,listenFd);


    if(bind(listenFd,(struct sockaddr*)&ss->serverAddr,sizeof(ss->serverAddr))<0){
        perror("socket bind error, exit!");
        exit(1);
    }

    if(listen(ss->listenFd,BACKLOG)<0){
        perror("socket listen error, exit!");
        exit(1);
    }

    while(true){
        if((connFd= accept(ss->listenFd,(struct sockaddr*)&clientAddr,&clientLength))<0){
            perror("accept function error!");
            continue;
        }
        connectStatus* cs=new connectStatus(string(inet_ntoa(clientAddr.sin_addr)),ntohs(clientAddr.sin_port),connFd);
        ss->Lock();
        //这里加锁，一直持有到进入recv_message函数中再释放，如果创建线程失败，直接释放
        ss->addConnect(connFd,cs);

        if(pthread_create(&recvTid , NULL , recv_message, &connFd) == -1)
        {
            perror("pthread create error.\n");
            ss->closeConnect(connFd);
            ss->unLock();
            continue;
        }//if
    }
}