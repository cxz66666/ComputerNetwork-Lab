//
// Created by ChenXuzheng on 2021/10/5.
//

#include "receive.h"
#include "../config.h"
#include "data.h"
#include "handler.h"
#include "util.h"

// just try to empty the buffer
bool recoveryFromFail(int connFd)
{
    printf("[server] An error occurred when receive! try to recovery it\n");
    try
    {
        char buf[MAXLINE + 5];
        int n;
        n = recv(connFd, buf, MAXLINE, 0);
        if (n == 0)
        {
            printf("[server] receive empty when recovery, try to disconnect\n");
            return false;
        }
        if (n < 0)
        {
            printf("[server] receive negative number when recovery, try to disconnect\n");
            return false;
        }
        while (n == MAXLINE)
        {
            n = recv(connFd, buf, MAXLINE, 0);
        }
    }
    catch (exception e)
    {
        printf("[server] This error can't be recovery, disconnect!\n");
        return false;
    }
    return true;
}
void *recv_message(void *fd)
{
    int connFd = *(int *)fd;
    pthread_t recvTid = pthread_self();
    ss->addThreadId(connFd, recvTid);
    printf("[server] accept a new connect, %s\n", ss->ss[connFd]->print().c_str());
    ss->unLock();
    while (true)
    {
        char buf[MAXLINE];
        memset(buf, 0, MAXLINE);
        int n;
        if ((n = recv(connFd, buf, TYPENUMBER, 0)) == -1)
        {
            printf("[server] receive error, error is %s, disconnect!\n", strerror(errno));
        }
        else if (n == 0)
        {
            printf("[server] receive empty from client %s, disconnect!\n", ss->ss[connFd]->print().c_str());
        }
        if (n <= 0)
        {
            ss->Lock();
            ss->closeConnect(connFd);
            ss->unLock();
            pthread_exit(0);
        }
        buf[n] = '\0';
        int type = string2int(buf);
        if (type == 0)
        {
            printf("[server] warning, type=0 is not legal\n");
        }
        printf("[server] accept Client request, type is %d\n", type);
        int success = false;
        switch (type)
        {
        case CONNECTNUMBER:
            success = false;
            break;
        case DISCONNECTNUMBER:
            success = false;
            break;
        case TIMENUMBER:
            success = handleTime(connFd);
            break;
        case NAMENUMBER:
            success = handleName(connFd);
            break;
        case LISTNUMBER:
            success = handleList(connFd);
            break;
        case SENDNUMER_SENDER:
            success = handleSend(connFd);

            break;
        default:
            success = false;
            printf("[server] accept a not legal type from %d, exit receive thread\n", connFd);
            break;
        }

        if (!success)
        {
            if (!recoveryFromFail(connFd))
            {
                ss->Lock();
                ss->closeConnect(connFd);
                ss->unLock();
                pthread_exit(0);
            }
        }
    }
}
