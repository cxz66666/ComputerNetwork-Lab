//
// Created by ChenXuzheng on 2021/10/5.
//

#include "../config.h"
#include "./recevice.h"
#include "./handler.h"
#include "./data.h"






//recvTid is the thread to receive
pthread_t recvTid=-1;

//sockFd is the client socket number
int sockFd=-1;

struct clientStatus* cs= nullptr;


int main(int argc, char **argv){
    cs=new clientStatus();

    printWelcome();

    vector<string>tmpCommands;
    while(true){
        tmpCommands.clear();
        cout<<"client>"<<flush;
        string tmp;
        getline(cin,tmp);
        // split string
        stringstream s(tmp);
        while(s>>tmp){
            tmpCommands.push_back(tmp);
        }
        if(!tmpCommands.size()){
            continue;
        }

        if(tmpCommands[0]==CONNECT){
            handleConnect(tmpCommands);
        } else if(tmpCommands[0]==DISCONNECT){
            handleDisConnect(tmpCommands);
        } else if(tmpCommands[0]==TIME){
            handleTime(tmpCommands);
        } else if(tmpCommands[0]==NAME){
            handleName(tmpCommands);
        } else if(tmpCommands[0]==LIST){
            handleList(tmpCommands);
        } else if(tmpCommands[0]==SEND){
            handleSend(tmpCommands);
        } else if(tmpCommands[0]==EXIT){
            handleExit(tmpCommands);
        } else if(tmpCommands[0]==HELP){
            handleHelp(tmpCommands);
        } else {
            printf("[client]: '%s' is not a legal command.\n"
                   "       See 'help'\n",tmpCommands[0].c_str());
        }

    }
}

