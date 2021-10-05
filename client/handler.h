//
// Created by root on 10/5/21.
//

#ifndef COMPUTERNETWORK_LAB_HANDLER_H
#define COMPUTERNETWORK_LAB_HANDLER_H

#include <string>
#include <vector>
using namespace std;
void printHelp();
void printWelcome();
void handleConnect(vector<string>&commands);
void handleDisConnect(vector<string>&commands);
void handleTime(vector<string>&commands);
void handleName(vector<string>&commands);
void handleList(vector<string>&commands);
void handleSend(vector<string>&commands);
void handleExit(vector<string>&commands);
void handleHelp(vector<string>&commands);
#endif //COMPUTERNETWORK_LAB_HANDLER_H
