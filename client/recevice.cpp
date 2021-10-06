//
// Created by ChenXuzheng on 2021/10/5.
//

#include "recevice.h"
#include "../config.h"
#include "data.h"
#include "util.h"

bool receiveStopFlag(int sockFd){
    char buf[2*STOPLENGTH];
    memset(buf,0,sizeof(buf));
    int n;
    if((n = recv(sockFd , buf , STOPLENGTH , 0)) == -1){
        printf("[server] receive stopFlag, error is %s, disconnect!\n", strerror(errno));
    } else if(n==0){
        printf("[server] receive empty from server, disconnect!\n");
    }
    if(strcmp(STOPFLAG.c_str(),buf)==0){
        return true;
    }
    return false;
}

bool receiveTime(int sockFd){
    char buf[MAXLINE];
    memset(buf,0,MAXLINE);
    int n;
    if((n=recv(sockFd,buf,4,0))==-1){
        printf("[client] receive error, error is %s\n", strerror(errno));
    } else if(n==0){
        printf("[client] receive empty from server, disconnect!\n");
    }
    int length= string2int(buf);

    if(n<=0||length==0){
        return false;
    }

    n=recv(sockFd,buf,length,0);
    printf("[client] receive time %s , bytes number is %d\n",buf,n);

    return receiveStopFlag(sockFd);
}

bool receiveName(int sockFd){
    char buf[MAXLINE];
    memset(buf,0,MAXLINE);
    int n;
    if((n=recv(sockFd,buf,4,0))==-1){
        printf("[client] receive error, error is %s\n", strerror(errno));
    } else if(n==0){
        printf("[client] receive empty from server, disconnect!\n");
    }
    int length= string2int(buf);
    if(n<=0||length==0){
        return false;
    }

    n=recv(sockFd,buf,length,0);
    printf("[client] receive name %s , bytes number is %d\n",buf,n);
    return receiveStopFlag(sockFd);
}

bool receiveList(int sockFd){
    char buf[MAXLINE];
    memset(buf,0,MAXLINE);
    int n;
    if((n=recv(sockFd,buf,4,0))==-1){
        printf("[client] receive error, error is %s\n", strerror(errno));
    } else if(n==0){
        printf("[client] receive empty from server, disconnect!\n");
    }
    int length= string2int(buf);
    if(n<=0||length==0){
        return false;
    }

    n=recv(sockFd,buf,length,0);
    printf("[client] receive List %s , bytes number is %d\n",buf,n);
    return receiveStopFlag(sockFd);
}

bool receiveSend(int sockFd){
    char buf[MAXLINE];
    memset(buf,0,MAXLINE);
    int n;
    if((n=recv(sockFd,buf,4,0))==-1){
        printf("[client] receive error, error is %s\n", strerror(errno));
    } else if(n==0){
        printf("[client] receive empty from server, disconnect!\n");
    }
    int length= string2int(buf);
    if(n<=0||length==0){
        return false;
    }

    n=recv(sockFd,buf,length,0);
    printf("[client] receive send %s , bytes number is %d\n",buf,n);
    return receiveStopFlag(sockFd);
}


void *recv_message(void *fd){
    int sockFdArgs=*(int*)fd;

    while(true){
        char buf[MAXLINE];
        memset(buf,0,MAXLINE);
        int n;
        if((n=recv(sockFdArgs,buf,TYPENUMBER,0))==-1){
            printf("[client] receive error, error is %s\n", strerror(errno));
        } else if(n==0){
            printf("[client] receive empty from server, disconnect!\n");
        }
        //need to exit the thread
        if(n<=0){
            cs->Lock();
            cs->init();
            sockFd=-1;
            recvTid=-1;
            close(sockFdArgs);
            cs->unLock();
            pthread_exit(0);
        }

        buf[n]='\0';
        int type=string2int(buf);
        printf("[client] receive %d type from server\n",type);
        switch (type) {
            case TIMENUMBER:
                receiveTime(sockFdArgs);
                break;
            case NAMENUMBER:
                receiveName(sockFdArgs);
                break;
            case LISTNUMBER:
                receiveList(sockFdArgs);
                break;
            case SENDNUMER:
                receiveSend(sockFdArgs);
                break;
            default:
                printf("[client] accept unknown type!\n");
        }
    }
}
