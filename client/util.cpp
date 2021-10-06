//
// Created by ChenXuzheng on 2021/10/6.
//

#include "util.h"


// num will be parse to 4 bits string, for example 65 will be 0x00 0x00 0x00 0x41;
string int2string(int number){
    string res;
    res+=char(number>>24);
    res+=char(number>>16);
    res+=char(number>>8);
    res+=char(number);
    return res;
}
int string2int(char *number){
    int ans=0;
    ans+=number[0]<<24;
    ans+=number[1]<<16;
    ans+=number[2]<<8;
    ans+=number[3];
    return ans;
}