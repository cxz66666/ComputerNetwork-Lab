//
// Created by root on 10/5/21.
//

#include "handler.h"
#include "data.h"
#include "recevice.h"
#include "../config.h"
#include "util.h"



void printHelp(){
    cout<<"\nUsage: [<Commands>] [<argument> ...]\n\n";
    cout<<"Named Commands:\n";
    cout<<"  connect [ip] [port]    connect to the given ip:host, usage: connect 127.0.0.1 4444\n";
    cout<<"  disconnect             disconnect if already connect to server\n";
    cout<<"  time                   get the server time if connected\n";
    cout<<"  name                   get the server name if connected\n";
    cout<<"  list                   show the connected list from server if connected\n";
    cout<<"  send [index]           send content to the client, you need input at next line, and use EOF to stop\n";
    cout<<"  exit                   disconnect and exit the program\n";
    cout<<"  help                   show the help list when you need or forget\n"<<endl;
}

void printWelcome(){
    cout<<"Welcome !"<<endl;
    printHelp();
}


void printInlegalParam(const char *name,int expect,int given){
    printf("[client] param number error, %s need %d args, but given %d\n",name,expect,given);
    return;
}

void handleConnect(vector<string>&commands){
    if(commands.size()!=3){
        printInlegalParam(CONNECT.c_str(),3,commands.size());
        return;
    }
    int port;
    try{
        port= stoi(commands[2]);
    } catch (exception e){
        printf("[client] connect need a int port, but got a illegal port\n");
        return;
    }
    cs->Lock();

    //already connected
    if(cs->hasConnected()){
        printf("[client] already connect!\n");
        cs->unLock();
        return;
    }

    //ip or port error
    if(!cs->setIpAndHost(commands[1].c_str(),port)){
        cs->init();
        printf("[client] connect get a illegal ip:host\n");
        cs->unLock();
        return;
    }
    if((sockFd= socket(AF_INET,SOCK_STREAM,0))==-1){
        perror("[client] socket init error, please try again\n");
        cs->init();
        cs->unLock();
        return;
    }
    if(connect(sockFd,(struct sockaddr*)&cs->serverAddr,sizeof(cs->serverAddr))<0){
        printf("[client] connect to server error, sockFd is %d\n",sockFd);
        cs->init();
        sockFd=-1;
        cs->unLock();
        return;
    }

    if(pthread_create(&recvTid,NULL,recv_message,&sockFd)==-1){
        printf("[client] received thread create error\n");
        cs->init();
        sockFd=-1;
        cs->unLock();
        return;
    }

    cs->connect= true;
    cs->printNowStatus();
    cs->unLock();
    return;
}

void handleDisConnect(vector<string>&commands){
    if(commands.size()>1){
        printInlegalParam(DISCONNECT.c_str(),3,commands.size());
        return;
    }
    cs->Lock();
    if(!cs->hasConnected()){
        printf("[client] don't connect yet!\n");
        cs->unLock();
        return;
    }
    if(recvTid>0){
        pthread_cancel(recvTid);
        recvTid=-1;
    }
    if(sockFd>0){
        close(sockFd);
        sockFd=-1;
    }
    cs->init();
    cs->unLock();
    printf("[client] disconnect!\n");
    return;
}
void handleTime(vector<string>&commands){
    if(commands.size()>1){
        printInlegalParam(TIME.c_str(),1,commands.size());
        return;
    }
    cs->Lock();
    if(!cs->hasConnected()){
        printf("[client] don't connect yet!\n");
        cs->unLock();
        return;
    }
    string sendStr=int2string(TIMENUMBER);
    sendStr+=STOPFLAG;

    send(sockFd,sendStr.c_str(), sendStr.size(),0);

    cs->unLock();
    return;
}
void handleName(vector<string>&commands){
    if(commands.size()>1){
        printInlegalParam(NAME.c_str(),1,commands.size());
        return;
    }
    cs->Lock();
    if(!cs->hasConnected()){
        printf("[client] don't connect yet!\n");
        cs->unLock();
        return;
    }
    string sendStr=int2string(NAMENUMBER);
    sendStr+=STOPFLAG;

    send(sockFd,sendStr.c_str(), sendStr.size(),0);

    cs->unLock();
    return;
}
void handleList(vector<string>&commands){
    if(commands.size()>1){
        printInlegalParam(LIST.c_str(),1,commands.size());
        return;
    }
    cs->Lock();
    if(!cs->hasConnected()){
        printf("[client] don't connect yet!\n");
        cs->unLock();
        return;
    }
    string sendStr=int2string(LISTNUMBER);
    sendStr+=STOPFLAG;

    send(sockFd,sendStr.c_str(), sendStr.size(),0);

    cs->unLock();
    return;
}
void handleSend(vector<string>&commands){
    if(commands.size()<3){
        printInlegalParam(LIST.c_str(),1,commands.size());
        return;
    }
    int connNum= stoi(commands[1]);
    if(connNum==0){
        printf("[client] parse connectFd error!\n");
        return;
    }
    cs->Lock();
    if(!cs->hasConnected()){
        printf("[client] don't connect yet!\n");
        cs->unLock();
        return;
    }
    //这个包的格式是 4bits type  4bits str.length （4bits connFd+content） 注意这个connFd长度包括在str.length内   STOPFLAG
    string content;
    for(int i=2;i<commands.size();i++){
        content+=commands[i]+' ';
    }
    content= int2string(connNum)+content;
    string sendStr= int2string(SENDNUMER_SENDER)+ int2string(content.size())+content+STOPFLAG;

    send(sockFd,sendStr.c_str(), sendStr.size(),0);

    cs->unLock();
    return;

}
void handleExit(vector<string>&commands){
    if(commands.size()>1){
        printInlegalParam(EXIT.c_str(),1,commands.size());
        return;
    }
    cs->Lock();
    if(cs->hasConnected()){
        if(recvTid>0){
            pthread_cancel(recvTid);
        }
        if(sockFd>0){
            close(sockFd);
        }
        printf("[client] disconnect!\n");
    }
    cs->unLock();
    printf("[client] ByeBye!\n");
    exit(0);
}
void handleHelp(vector<string>&commands){
    printHelp();
    return;
}