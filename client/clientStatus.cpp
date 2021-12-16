//
// Created by root on 10/5/21.
//

#include "clientStatus.h"
#include "../config.h"
#include "data.h"
#include "recevice.h"

clientStatus::clientStatus()
{
    init();
}

bool clientStatus::setIpAndHost(const char *ip, int port)
{
    const char *defaultIP = "127.0.0.1"; // just use for test!

    if (inet_pton(AF_INET, ip, &serverAddr.sin_addr) <= 0)
    {
        return false;
    }
    if (port < 0 || port > 65535)
    {
        return false;
    }
    serverAddr.sin_port = htons(port);
    return true;
}

bool clientStatus::hasConnected()
{
    return isConnect;
}

void clientStatus::init()
{
    isConnect = false;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    sockFd = -1;
    recvTid = -1;
}
string clientStatus::getServer()
{
    if (!isConnect)
    {
        return "not connect to server yet!";
    }
    else
    {
        char buf[100];
        sprintf(buf, "ip:%s, port:%d", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));
        return string(buf);
    }
    return "";
};
void clientStatus::printNowStatus()
{
    if (!isConnect)
    {
        printf("[status] not connect yet!\n");
    }
    else
    {
        printf("[status] connected! server ip: %s, port: %d\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));
    }
    return;
}

void clientStatus::Lock()
{
    csMutex.lock();
    return;
}

void clientStatus::unLock()
{
    csMutex.unlock();
    return;
}

void clientStatus::initAndClose()
{
    cs->init();
    if (sockFd > 0)
    {
        close(sockFd);
    }
    sockFd = -1;
    recvTid = -1;
}

int clientStatus::sockFdCreate()
{
    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    return sockFd;
}

int clientStatus::recvTidCreate(void *(*rec_message)(void *))
{
    pthread_create(&recvTid, NULL, rec_message, &sockFd);
    return recvTid;
}

int clientStatus::connectServer()
{
    return connect(sockFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
}

void clientStatus::disconnectServer()
{
    if (cs->hasConnected())
    {
        if (recvTid > 0)
        {
            pthread_cancel(recvTid);
        }
        if (sockFd > 0)
        {
            close(sockFd);
        }
        init();
    }
    else
    {
        printf("[client] don't isConnect yet!\n");
    }
    return;
}
