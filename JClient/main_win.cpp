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
#define MAXLINE 4096
#define MAXDATA 8192

void Useage(){
    std::string  str= "---------command---------\n"
                      "1 : 播放\n"
                      "2 : 暂停\n"
                      "3 : 音量+5\n"
                      "4 : 音量-5\n"
                      "5 : 音量设置 + 0~100\n"
                      "6 : 重播\n"
                      "7 : 停止\n"
                      "8 : 选择文件播放 + 文件序号\n"
                      "--------------------------\n"
                      "示例> 8  test.mp4\n";
    printf(str.c_str());
}

void PrintPkg(PCPackage & pkg){
    std::string msg = "--------------------------------\n";
    switch (pkg._pact) {
        case PAction::EM_NOTIFY:{
            msg += "服务器已接收\n";
        } break;
        case PAction::EM_NOTIFY_PLAYED:{
            msg += "视频(" + pkg._content + ")播放完毕\n";
        } break;
        case PAction::EM_ERROR:{
            msg += "错误：\n";
            msg += pkg._content + "\n";
        } break;
        case PAction::EM_NOTIFY_FILES:{
            msg += "服务端文件列表：\n";
            std::vector<std::string> vec;
            Comm::SplitFilename(vec,pkg._content);
            for (int i = 0; i < vec.size(); ++i) {
                msg += std::to_string(i+1) + " : "+vec[i] + "\n";
            }
        } break;
        default:{
            msg += "无效的消息\n";
        }
    }
    printf("%s\n",msg.c_str());
    fflush(stdout);
}

PCMsg parser(std::string & src){
    PCMsg ret;
    std::vector<std::string> cmd;
    Comm::SplitString(cmd,src," ");
    std::string cmd1;
    std::string cmd2;
    if(cmd.size()==2){
        cmd1 = cmd[0];
        cmd2 = cmd[1];
    } else if(cmd.size()==1){
        cmd1 = cmd[0];
    } else {
        return ret;
    }

    int icmd = std::stoi(cmd1);
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
        char recData[MAXDATA];
        int ret = recv(*sock, recData, MAXDATA, 0);
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
                PrintPkg(pkg);
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
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
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
