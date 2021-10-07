//
// Created by ChenXuzheng on 2021/10/5.
//

#include "recevice.h"
#include "../config.h"
#include "data.h"
#include "util.h"

// use in all other receive fuctions to receive the STOPFLAG
bool receiveStopFlag(int sockFd){
    char buf[2*STOPLENGTH];
    memset(buf,0,sizeof(buf));
    int n;
    if((n = recv(sockFd , buf , STOPLENGTH , 0)) == -1){
        printf("[server] receive stopFlag, error is %s, disconnect!\n", strerror(errno));
    } else if(n==0){
        printf("[server] receive empty from server %s:, disconnect!\n",cs->getServer().c_str());
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
    printf("[client] receive time: %s \n",buf);

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
    printf("[client] receive name: %s\n",buf);
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
    printf("[client] receive List following----\n%s\n",buf);
    return receiveStopFlag(sockFd);
}

bool receiveSendSender(int sockFd){
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
    printf("[client] send message status: %s\n",buf);
    return receiveStopFlag(sockFd);
}
bool receiveSendReceiver(int sockFd){
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
    printf("[client] receive from client message: %s\n",buf);
    return receiveStopFlag(sockFd);
}

//just try to empty the buffer
bool recoveryFromFail(int connFd){
    printf("[client] An error occurred when receive! try to recovery it\n");
    try{
        char buf[MAXLINE+5];
        int n;
        n=recv(connFd,buf,MAXLINE,0);
        if(n==0){
            printf("[client] receive empty when recovery, try to disconnect\n");
            return false;
        }
        if(n<0){
            printf("[client] receive negative number when recovery, try to disconnect\n");
            return false;
        }
        while(n==MAXLINE){
            n=recv(connFd,buf,MAXLINE,0);
        }
    } catch(exception e){
        printf("[client] This error can't be recovery, disconnect!\n");
        return false;
    }

    return true;
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
            cs->initAndClose();
            cs->unLock();
            pthread_exit(0);
        }

        buf[n]='\0';
        int type=string2int(buf);
        printf("[client] receive %d type from server\n",type);
        bool success=false;
        try{
            switch (type) {
                case TIMENUMBER:
                    success= receiveTime(sockFdArgs);
                    break;
                case NAMENUMBER:
                    success= receiveName(sockFdArgs);
                    break;
                case LISTNUMBER:
                    success= receiveList(sockFdArgs);
                    break;
                case SENDNUMER_SENDER:
                    success= receiveSendSender(sockFdArgs);
                    break;
                case SENDNUMER_RECEIVER:
                    success= receiveSendReceiver(sockFdArgs);
                    break;
                default:
                    success=false;
                    printf("[client] accept unknown type, try to recovery!\n");
            }
        } catch(exception e){
            success=false;
            printf("[client] An error occurred when receive content, try to recovery!\n");
        }

        if(!success){
            //try to recovery, and if it failed, close and disconnect
            if(!recoveryFromFail(sockFdArgs)){
                cs->Lock();
                cs->initAndClose();
                cs->unLock();
                pthread_exit(0);
            }
        }
    }
}
