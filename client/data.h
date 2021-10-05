//
// Created by root on 10/5/21.
//

#ifndef COMPUTERNETWORK_LAB_DATA_H
#define COMPUTERNETWORK_LAB_DATA_H

#include "clientStatus.h"
// all this data are initial in client.cpp
//recvTid is the thread to receive
extern pthread_t recvTid;

//sockFd is the client socket number
extern int sockFd;



extern struct clientStatus* cs;


#endif //COMPUTERNETWORK_LAB_DATA_H
