//
// Created by ChenXuzheng on 2021/10/5.
//

#ifndef COMPUTERNETWORK_LAB_SERVERSTATUS_H
#define COMPUTERNETWORK_LAB_SERVERSTATUS_H

#include "../config.h"

//表示一个连接状态
class connectStatus{
public:
    string ip;
    int port;
    int connFd;
    //接收/发送线程的threadId
    pthread_t recvTid;
    connectStatus(const string &ip, int port, int connFd,pthread_t recv);

    //大部分时候创建该结构体的时候recvTid还没拿到,所以先弄个三个构造函数的
    connectStatus(const string &ip, int port, int connFd);

    connectStatus();

    //toString return a string contains all the information
    string toString();
};

//表示server的状态结构体
class serverStatus{
public:
    //key is the current connFd
    map<int,connectStatus*>ss;

    string serverName;
    int listenFd;
    struct sockaddr_in serverAddr;

    mutex ssMutex;

    serverStatus(const string &serverName, int listenFd);
    serverStatus();
    void Lock();
    void unLock();
    //关闭一个连接
    void closeConnect(int connFd);
    //增加一个连接
    void addConnect(int connFd,connectStatus*cs);
    //增加一个连接中的threadId
    void addThreadId(int connFd, pthread_t tid);
};



#endif //COMPUTERNETWORK_LAB_SERVERSTATUS_H
