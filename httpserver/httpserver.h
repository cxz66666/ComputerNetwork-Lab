//
// Created by ChenXuzheng on 2021/12/16.
//

#ifndef HTTPSERVER_HTTPSERVER_H
#define HTTPSERVER_HTTPSERVER_H

#include <iostream>
#include <vector>
#include <fstream>
#include <netinet/in.h>
#include <string>
#include <unistd.h>
#include "config.h"
#include <arpa/inet.h>
#include <regex>


using namespace std;


void handleConnect(int cfd);

string get(const string &path, const string &http);

string post(const string &path, const string &http, const string &data);

string makeHttpReply(const string &data = "", const int &code = 200, const string &contentType = "text/html");

string replyRequest(const string &request);

class httpserver {
private:
    int socketFd;
    sockaddr_in sin;


public:
    ~httpserver();

    explicit httpserver(int port, string rootPath);

    void mainLoop() const;

    static void *handleThread(void *fd) {
        handleConnect(*(int *) fd);
        return NULL;
    }
};

#endif //HTTPSERVER_HTTPSERVER_H
