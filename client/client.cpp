//
// Created by ChenXuzheng on 2021/10/5.
//

#include "../config.h"
#include "./recevice.h"
#include "./handler.h"
#include "./data.h"
#include "./clientStatus.h"
//输入需要加上ip地址参数
//usage: ./client 127.0.0.1





void printHelp(){
    cout<<"\nUsage: [<Commands>] [<argument> ...]\n\n";
    cout<<"Named Commands:\n";
    cout<<"  connect [ip] [port]    connect to the given ip:host, usage: connect 127.0.0.1 4444\n";
    cout<<"  disconnect             disconnect if already connect to server\n";
    cout<<"  time                   get the server time if connected\n";
    cout<<"  name                   get the server name if connected\n";
    cout<<"  list                   show the connected list from server if connected\n";
    cout<<"  send [index]           send content to the client, you need input at next line, and use EOF to stop\n";
    cout<<"  exit                   disconnect and exit the program\n";
    cout<<"  help                   show the help list when you need or forget\n"<<endl;
}

void printWelcome(){
    cout<<"Welcome !"<<endl;
    printHelp();
}

//recvTid is the thread to receive
pthread_t recvTid=-1;

//sockFd is the client socket number
int sockFd=-1;

struct clientStatus* cs= nullptr;


int main(int argc, char **argv){
    struct clientStatus* cs=new clientStatus();

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
            printf("[client]: '%s' is not a legal command.\nSee 'help'\n",tmpCommands[0].c_str());
        }

    }
}

