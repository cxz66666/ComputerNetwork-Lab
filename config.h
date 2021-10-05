//
// Created by ChenXuzheng on 2021/10/5.
//

#ifndef COMPUTERNETWORK_LAB_CONFIG_H
#define COMPUTERNETWORK_LAB_CONFIG_H
#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>


using namespace std;

const int PORT=4611;
const string SERVERNAME="raynor-HP";
//BACKLOG is the max number of queue
const int BACKLOG=40;
const int MAXLINE=2048;


const string CONNECT="connect";
const string DISCONNECT="disconnect";
const string TIME="time";
const string NAME="name";
const string LIST="list";
const string SEND="send";
const string EXIT="exit";
const string HELP="help";


const int TYPENUMBER=4;

//connect 和 disconnect基本没用，此连接的时候非常简单，直接connect，关闭的时候则close套接字，对方就会发现recv长度为0，被动关闭
const int CONNECTNUMBER=1;
const int DISCONNECTNUMBER=2;
const int TIMENUMBER=3;
const int NAMENUMBER=4;
const int LISTENNUMBER=5;
const int SENDNUMER=6;
const int EXITNUMBER=7;


#endif //COMPUTERNETWORK_LAB_CONFIG_H
