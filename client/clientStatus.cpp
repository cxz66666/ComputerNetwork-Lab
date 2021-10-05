//
// Created by root on 10/5/21.
//

#include "clientStatus.h"
#include "../config.h"
clientStatus::clientStatus() {
    init();
}

bool clientStatus::setIpAndHost(const char *ip, int port) {
    const char *defaultIP = "127.0.0.1";

    if(inet_pton(AF_INET,defaultIP,&serverAddr.sin_addr)<=0){
        return false;
    }
    if(port<0||port>65535){
        return false;
    }
    serverAddr.sin_port= htons(port);
    return true;
}

bool clientStatus::hasConnected() {
    return connect;
}

void clientStatus::init() {
    connect= false;
    bzero(&serverAddr,sizeof(serverAddr));
    serverAddr.sin_family=AF_INET;
}

void clientStatus::printNowStatus() {
    if(!connect){
        printf("[status] not connect yet!\n");
    } else {
        printf("[status] connected! server ip: %s, port: %d\n",inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));
    }
    return;
}

void clientStatus::Lock(){
    csMutex.lock();
    return;
}

void clientStatus::unLock() {
    csMutex.unlock();
    return;
}