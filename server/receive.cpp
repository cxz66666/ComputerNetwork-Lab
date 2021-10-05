//
// Created by ChenXuzheng on 2021/10/5.
//

#include "receive.h"
#include "../config.h"
void *recv_message(void *fd){
    int sockFd=*(int*)fd;
    while(true){
        char buf[MAXLINE];
        memset(buf,0,MAXLINE);
        int n;
        if((n = recv(sockFd , buf , MAXLINE , 0)) == -1)
        {
            perror("[server] recv error.\n");
            pthread_exit(0);
        }//if
        buf[n] = '\0';
        //若收到的是exit字符，则代表退出通信
        if(strcmp(buf, "byebye.") == 0)
        {
            printf("[server] Client closed.\n");
            close(sockFd);
            pthread_exit(0);
        }//if

        printf("[server] accept Client res: %s\n", buf);
        if(send(sockFd,buf, strlen(buf),0)==-1){
            perror("[server] send error.\n");
            pthread_exit(0);
        }
    }
}