//
// Created by root on 10/5/21.
//

#ifndef COMPUTERNETWORK_LAB_CLIENTSTATUS_H
#define COMPUTERNETWORK_LAB_CLIENTSTATUS_H

#include <arpa/inet.h>
#include <mutex>
#include <netinet/in.h>
#include <pthread.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
using namespace std;
class clientStatus
{
public:
    bool isConnect;
    sockaddr_in serverAddr;
    mutex csMutex;

    // sockFd is the client socket number
    int sockFd;
    // recvTid is the thread to receive
    pthread_t recvTid;

    clientStatus();
    // set the current ip and port to
    bool setIpAndHost(const char *ip, int port);
    bool hasConnected();
    void init();
    void initAndClose();
    // just print the
    string getServer();
    // print now status
    void printNowStatus();
    void Lock();
    void unLock();

    // create the sockFd and return sockFd whether success or not
    int sockFdCreate();

    // try to connect to server with the sockFd and serverAddr
    int connectServer();

    // try to disconnect to server and free sockFd and thread
    void disconnectServer();

    // create the receiveThread use the function
    int recvTidCreate(void *(*rec_message)(void *));
};

#endif // COMPUTERNETWORK_LAB_CLIENTSTATUS_H
