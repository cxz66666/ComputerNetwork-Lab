//
// Created by ChenXuzheng on 2021/10/5.
//

#include "recevice.h"
#include "../config.h"
void *recv_message(void *fd){
    int sockFd=*(int*)fd;
    while(true){
        char buf[MAXLINE];
        memset(buf,0,MAXLINE);
        int n;
        if((n=recv(sockFd,buf,MAXLINE,0))==-1){
            perror("receive error.\n");
            pthread_exit(0);
        }
        printf("[client] accept total %d bytes\n",n);
        buf[n]='\0';
        if(strcmp(buf , "byebye.") == 0)
        {
            printf("Server is closed.\n");
            close(sockFd);
            pthread_exit(0);
        }//if
        printf("[client] accept contents:%s",buf);
    }
}