//
// Created by ChenXuzheng on 2021/12/16.
//
#include "httpserver.h"

int main() {
    httpserver hp(PORT, ROOTPATH);
    hp.mainLoop();
}
