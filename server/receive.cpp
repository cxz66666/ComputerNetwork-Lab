//
// Created by ChenXuzheng on 2021/10/5.
//

#include "receive.h"
#include "data.h"
#include "../config.h"
#include "handler.h"
#include "util.h"

void *recv_message(void *fd){
    int connFd=*(int*)fd;
    pthread_t recvTid=pthread_self();
    ss->addThreadId(connFd,recvTid);
    printf("[server] accept a new connect, %s\n",ss->ss[connFd]->print().c_str());
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
            pthread_exit(0);
        }
        buf[n] = '\0';
        int type= string2int(buf);
        if(type==0){
            printf("[server] warning, type=0 is not legal\n");
        }
        printf("[server] accept Client request, type is %d\n",type);
        switch (type) {
            case CONNECTNUMBER:
                break;
            case DISCONNECTNUMBER:
                break;
            case TIMENUMBER:
                if(!handleTime(connFd)){
                    ss->Lock();
                    ss->closeConnect(connFd);
                    ss->unLock();
                    pthread_exit(0);
                }
                break;
            case NAMENUMBER:
                if(!handleName(connFd)){
                    ss->Lock();
                    ss->closeConnect(connFd);
                    ss->unLock();
                    pthread_exit(0);
                }
                break;
            case LISTNUMBER:
                if(!handleList(connFd)){
                    ss->Lock();
                    ss->closeConnect(connFd);
                    ss->unLock();
                    pthread_exit(0);
                }
                break;
            case SENDNUMER:
                if(!handleSend(connFd)){
                    ss->Lock();
                    ss->closeConnect(connFd);
                    ss->unLock();
                    pthread_exit(0);
                }
                break;
            default:
                printf("[server] accept a not legal type from %d, exit receive thread\n",connFd);
                break;
        }
    }
}

