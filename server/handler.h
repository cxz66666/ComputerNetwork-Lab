//
// Created by ChenXuzheng on 2021/10/6.
//

#ifndef COMPUTERNETWORK_LAB_HANDLER_H
#define COMPUTERNETWORK_LAB_HANDLER_H
#include "data.h"
bool handleTime(int connFd);
bool handleName(int connFd);
bool handleList(int connFd);
bool handleSend(int connFd);

#endif //COMPUTERNETWORK_LAB_HANDLER_H
