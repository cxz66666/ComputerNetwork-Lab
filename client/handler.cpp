//
// Created by root on 10/5/21.
//

#include "handler.h"
#include "data.h"
#include "clientStatus.h"
#include "recevice.h"
#include "../config.h"
void handleConnect(vector<string>&commands){
    if(commands.size()!=3){
        printf("[client] connect need 3 args, but you give %ld\n",commands.size());
        return;
    }
    int port;
    try{
        port= stoi(commands[2]);
    } catch (exception e){
        printf("[client] connect need a int port, but got a illegal port\n");
        return;
    }
    //ip or port error
    if(!cs->setIpAndHost(commands[1].c_str(),port)){
        cs->init();
        printf("[client] connect get a illegal ip:host\n");
        return;
    }
    if((sockFd= socket(AF_INET,SOCK_STREAM,0))==-1){
        perror("[client] socket init error, please try again\n");
        cs->init();
        return;
    }
    if(connect(sockFd,(struct sockaddr*)&cs->serverAddr,sizeof(cs->serverAddr))<0){
        printf("[client] connect to server error, sockFd is %d\n",sockFd);
        cs->init();
        sockFd=-1;
        exit(1);
    }

    if(pthread_create(&recvTid,NULL,recv_message,&sockFd)==-1){
        printf("[client] received thread create error\n");
        cs->init();
        sockFd=-1;
    }

    cs->connect= true;
    cs->printNowStatus();
    return;
}

void handleDisConnect(vector<string>&commands){

}
void handleTime(vector<string>&commands){

}
void handleName(vector<string>&commands){

}
void handleList(vector<string>&commands){

}
void handleSend(vector<string>&commands){

}
void handleExit(vector<string>&commands){

}
void handleHelp(vector<string>&commands){

}