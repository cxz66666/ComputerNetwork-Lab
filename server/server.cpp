//
// Created by ChenXuzheng on 2021/10/5.
//

#include "../config.h"
#include "receive.h"

int main(){
    //监听的套接字
    int listenFd,connFd;
    pthread_t recvTid,sendTid;
    //地址的套接字
    struct sockaddr_in serverAddr, clientAddr;
    //用于传参的常数
    socklen_t clientLength=sizeof(clientAddr);

    if((listenFd= socket(AF_INET,SOCK_STREAM,0))==-1){
        perror("socket init error, exit!");
        exit(1);
    }

    bzero(&serverAddr,sizeof(serverAddr));
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_addr.s_addr= htonl(INADDR_ANY);
    serverAddr.sin_port= htons(PORT);

    if(bind(listenFd,(struct sockaddr*)&serverAddr,sizeof(serverAddr))<0){
        perror("socket bind error, exit!");
        exit(1);
    }

    if(listen(listenFd,BACKLOG)<0){
        perror("socket listen error, exit!");
        exit(1);
    }

    while(true){
        if((connFd= accept(listenFd,(struct sockaddr*)&clientAddr,&clientLength))<0){
            perror("accept function error!");
            continue;
        }
        printf("[server]: got connection from %s, port is %d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

        if(pthread_create(&recvTid , NULL , recv_message, &connFd) == -1)
        {
            perror("pthread create error.\n");
            exit(1);
        }//if
    }
}