//
// Created by ChenXuzheng on 2021/10/5.
//

#include "recevice.h"
#include "../config.h"
#include "data.h"


void *recv_message(void *fd){
    int sockFdArgs=*(int*)fd;

    while(true){
        char buf[MAXLINE];
        memset(buf,0,MAXLINE);
        int n;
        if((n=recv(sockFdArgs,buf,MAXLINE,0))==-1){
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
            pthread_exit(1);
        }

        printf("[client] accept total %d bytes\n",n);
        buf[n]='\0';
        if(strcmp(buf , "byebye.") == 0)
        {
            printf("Server is closed.\n");
            close(sockFdArgs);
            pthread_exit(0);
        }//if
        printf("[client] accept contents:%s",buf);
    }
}