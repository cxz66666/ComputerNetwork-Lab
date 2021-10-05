//
// Created by ChenXuzheng on 2021/10/5.
//

#include "receive.h"
#include "data.h"
#include "../config.h"

void *recv_message(void *fd){
    int connFd=*(int*)fd;
    pthread_t recvTid=pthread_self();
    ss->addThreadId(connFd,recvTid);
    ss->unLock();
    while(true){
        char buf[MAXLINE];
        memset(buf,0,MAXLINE);
        int n;
        if((n = recv(connFd , buf , TYPENUMBER , 0)) == -1){
            printf("[server] receive error, error is %s, disconnect!\n", strerror(errno));
        } else if(n==0){
            printf("[server] receive empty from server, disconnect!\n");
        }
        if(n<=0){
            ss->Lock();
            ss->closeConnect(connFd);
            ss->unLock();
            pthread_exit(1);
        }
        buf[n] = '\0';
        int type= atoi(buf);
        if(type==0){
            printf("[server] warning, type=0 is not legal\n");
        }
        switch (type) {
            case CONNECTNUMBER:
                break;
            case DISCONNECTNUMBER:
                break;
            case TIMENUMBER:
                break;
            case NAMENUMBER:
                break;
            case LISTNUMBER:
                break;
            case SENDNUMER:
                break;
            case
        }
        //若收到的是exit字符，则代表退出通信
        if(strcmp(buf, "byebye.") == 0)
        {
            printf("[server] Client closed.\n");
            close(connFd);
            pthread_exit(0);
        }//if

        printf("[server] accept Client res: %s, number is %d\n", buf,n);
        if(send(connFd,buf, strlen(buf),0)==-1){
            perror("[server] send error.\n");
            pthread_exit(0);
        }
    }
}