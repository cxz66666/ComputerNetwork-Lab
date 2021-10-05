//
// Created by root on 10/5/21.
//

#ifndef COMPUTERNETWORK_LAB_CLIENTSTATUS_H
#define COMPUTERNETWORK_LAB_CLIENTSTATUS_H

#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>
#include <mutex>
using namespace std;
class clientStatus{
public:
    bool connect;
    sockaddr_in serverAddr;
    mutex csMutex;
    clientStatus();
    //set the current ip and port to
    bool setIpAndHost(const char *ip,int port);
    bool hasConnected();
    void init();
    //print now status
    void printNowStatus();
    void Lock();
    void unLock();
};


#endif //COMPUTERNETWORK_LAB_CLIENTSTATUS_H
