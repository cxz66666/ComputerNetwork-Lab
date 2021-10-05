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

#endif //COMPUTERNETWORK_LAB_CONFIG_H
