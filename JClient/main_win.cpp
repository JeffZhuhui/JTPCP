//
// Created by jhzhu on 9/2/2023.
//


#include <WINSOCK2.H>
#include <STDIO.H>
#include <string>
#include <string.h>
#include "PlayControlAct.h"
#include "tool.h"
#include <thread>


#pragma comment(lib, "ws2_32")
#pragma  comment(lib,"ws2_32.lib")
#define MAXLINE 1024

void Useage(){
    std::string  str= "---------command---------\n"
                      "1 : 播放\n"
                      "2 : 暂停\n"
                      "3 : 音量+5\n"
                      "4 : 音量-5\n"
                      "5 : 音量设置 + 0~100\n"
                      "6 : 重播\n"
                      "7 : 停止\n"
                      "8 : 选择文件播放 + 文件名\n"
                      "--------------------------\n"
                      "示例> 8  test.mp4\n";
    printf(str.c_str());
}

PCMsg parser(std::string & src){
    PCMsg ret;
    std::vector<std::string> cmd;
    Comm::SplitString(cmd,src," ");
    if(cmd.size()!=2){
        return ret;
    }
    std::string cmd1 = cmd[0];
    int icmd = std::stoi(cmd1);
    std::string cmd2 = cmd[1];

    if(icmd > 8 || icmd < 1){
        return ret;
    }
    PCPackage pkg;
    pkg._pact = PAction(icmd);
    pkg._content = cmd2;

    Execption e;
    ret = PlayControlAct::Marshal(pkg,e);
    if(!e._error.empty()){
        printf("%s\n",e._error.c_str());
    }
    return ret;
}

void Read(SOCKET * sock){
    if(!sock) return;
    while(1){
        char recData[255];
        int ret = recv(*sock, recData, 255, 0);
        if(ret > 0)
        {
            recData[ret] = 0x00;
            PCMsg msg;
            for (int i = 0; i < ret; ++i) {
                msg._data[i] = recData[i];
            }
            msg._len = ret;

            printf("---------------Read--------------\n");
            Execption e;
            PCPackage pkg = PlayControlAct::Unmarshal(msg,e);
            if(!e._error.empty()){
                printf("read msg is invalid\n");
                fflush(stdout);
            } else {
                std::string strmsg = std::to_string(int(pkg._pact)) + " " + pkg._content;
                printf("%s\n",strmsg.c_str());
                fflush(stdout);
            }
        }
    }
}


int main(int argc, char* argv[])
{
    WORD sockVersion = MAKEWORD(2,2);
    WSADATA data;
    if(WSAStartup(sockVersion, &data) != 0)
    {
        return 0;
    }

    SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sclient == INVALID_SOCKET)
    {
        printf("invalid socket !\n");
        return 0;
    }

    std::string ip;
    int port;
    {
        printf("please input server ip:\n");
        fflush(stdout);
        char buff[MAXLINE];
        fgets(buff,MAXLINE,stdin);
        strtok(buff,"\n");
        ip = buff;
    }

    {
        printf("please input server port:\n");
        fflush(stdout);
        char buff[MAXLINE];
        fgets(buff,MAXLINE,stdin);
        strtok(buff,"\n");
        std::string sport = buff;
        port = stoi(sport);
    }


    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(port);
    serAddr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
    if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
    {
        printf("connect error !");
        closesocket(sclient);
        return 0;
    }

    Useage();

    std::thread thread(Read,&sclient);
    thread.detach();


    while(1){
        std::string senddata;
        {
            fflush(stdout);
            printf("please input cmd:\n");
            fflush(stdout);
            char buff[MAXLINE];
            fgets(buff,MAXLINE,stdin);
            strtok(buff,"\n");
            std::string cmd = buff;
            if(cmd == "quit" || cmd == "exit"){
                break;
            }
            PCMsg msg = parser(cmd);
            if(msg._len > 0){
                printf("---------------Write-------------\n");
                for (int i = 0; i < msg._len; ++i) {
                    char c = msg._data[i];
                    printf("%x ",c);
                }
                printf("\n");
                fflush(stdout);
                send(sclient, (char*)&(msg._data.front()), msg._len, 0);
            }
        }
    }


/*    char recData[255];
    int ret = recv(sclient, recData, 255, 0);
    if(ret > 0)
    {
        recData[ret] = 0x00;
        printf(recData);
    }*/
    closesocket(sclient);
    WSACleanup();
    return 0;
}
