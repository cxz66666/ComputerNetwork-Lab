//
// Created by ChenXuzheng on 2021/10/5.
//

#include "serverStatus.h"
#include <sstream>
connectStatus::connectStatus(const string &ip, int port, int connFd, pthread_t recv) : ip(ip), port(port), connFd(connFd), recvTid(recv)
{
}
connectStatus::connectStatus(const string &ip, int port, int connFd) : ip(ip), port(port), connFd(connFd)
{
}

connectStatus::connectStatus()
{
}

string connectStatus::print()
{
    //    char tmp[100];
    //    sprintf(tmp,"%s:%04d, connectFd is %d",ip.c_str(),port,connFd);
    //    return string(tmp);

    stringstream ss;
    ss << "connFd:" << connFd << "   address:" << ip << ":" << port << "\n";
    return ss.str();
}

serverStatus::serverStatus() {}

serverStatus::serverStatus(const string &serverName, int listenFd) : serverName(
                                                                         serverName),
                                                                     listenFd(listenFd)
{
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(PORT);
}
void serverStatus::Lock()
{
    ssMutex.lock();
}

void serverStatus::unLock()
{
    ssMutex.unlock();
}

void serverStatus::closeConnect(int connFd)
{
    if (ss.count(connFd))
    {
        char *msg = "connect closed by server\n";
        send(connFd, msg, strlen(msg), 0);
        close(connFd);
        connectStatus *cs = ss[connFd];
        if (cs)
        {
            delete cs;
        }
        ss.erase(connFd);
    }
    return;
}

void serverStatus::addConnect(int connFd, connectStatus *cs)
{
    ss[connFd] = cs;
    return;
}

void serverStatus::addThreadId(int connFd, pthread_t tid)
{
    if (ss.count(connFd))
    {
        ss[connFd]->recvTid = tid;
    }
    else
    {
        printf("[server] warning, add threadId to a not exist ID!\n");
    }
}

string serverStatus::getSnapshot()
{
    stringstream res;
    res << "Now have " << ss.size() << " clients connected\n";
    for (auto m = ss.begin(); m != ss.end(); m++)
    {
        res << m->second->print();
    }
    return res.str();
}
