//
// Created by ChenXuzheng on 2021/10/6.
//

#include "handler.h"
#include "util.h"
#include "data.h"


//getNowTime return a string like `10/06/21 02:58:31.914065283 UTC`
string getNowTime(){
    timespec ts;
    timespec_get(&ts, TIME_UTC);
    char buff[30];
    char res[50];
    strftime(buff, sizeof buff, "%D %T", std::gmtime(&ts.tv_sec));
    sprintf(res,"%s.%09ld UTC",buff, ts.tv_nsec);
    return string(res);
}


bool receiveStopFlag(int connFd){
    char buf[2*STOPLENGTH];
    memset(buf,0,sizeof(buf));
    int n;
    if((n = recv(connFd , buf , STOPLENGTH , 0)) == -1){
        printf("[server] receive stopFlag, error is %s, disconnect!\n", strerror(errno));
    } else if(n==0){
        printf("[server] receive empty from server, disconnect!\n");
    }
    if(strcmp(STOPFLAG.c_str(),buf)==0){
        return true;
    }
    return false;
}

//返回包的格式为 4bits TYPE 4bits length 若干bits string表示的time  结尾符
bool handleTime(int connFd){
    if(!receiveStopFlag(connFd)){
        return false;
    }
    string res= int2string(TIMENUMBER);
    string time=getNowTime();
    string number= int2string(time.size());
    res+=number+time+STOPFLAG;

    send(connFd,res.c_str(),res.size(),0);
    return true;
}
bool handleName(int connFd){
    if(!receiveStopFlag(connFd)){
        return false;
    }
    string res=int2string(NAMENUMBER);

    string number=int2string(ss->serverName.size());
    res+=number+ss->serverName;
    send(connFd,res.c_str(),res.size(),0);
    return true;
}
bool handleList(int connFd){

}
bool handleSend(int connFd){

}