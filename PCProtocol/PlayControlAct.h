//
// Created by jhzhu on 9/1/2023.
//

#ifndef PROD_PLAYCONTROLACT_H
#define PROD_PLAYCONTROLACT_H
#include <string>
#include <vector>

enum class PAction{
    EM_UNKNOWN,  //
    EM_PLAY = 1, //播放
    EM_PAUSE,   //暂停
    EM_VOL_ADD, ////音量+
    EM_VOL_SUB,//音量-
    EM_VOL_SET,//音量设置 + 0~100
    EM_REPLAY,//重播
    EM_STOP,//停止
    EM_SELECT,//选择文件播放 + 文件名
    EM_NOTIFY,//消息通知    服务器返回给客户端，表示命令已接收
    EM_NOTIFY_PLAYED,//播放完一个文件 通知客户端 + 文件名
    EM_ERROR,//错误消息 + 错误描述
    EM_NOTIFY_FILES//  当客户端连接成功之后   服务端返回文件列表中所有文件   文件名以 反斜杠 分割：  a.mp4\b.mp4\c.mp4
};

class PCPackage{
public:
    PAction _pact = PAction::EM_UNKNOWN;
    unsigned _packlen = 0;
    std::string _content;
};

#define MAXPACKSIZE 8192
class PCMsg{
public:
    PCMsg(){
        _data = std::vector<unsigned char>(MAXPACKSIZE);
    }
    std::vector<unsigned char> _data;
    unsigned _len = 0;
};

class Execption{
public:
    std::string _error;
};


class PlayControlAct {
public:
    static PCPackage Unmarshal(const PCMsg& msg, Execption & exc);
    static PCMsg Marshal(const PCPackage & pkg, Execption & exc);
};


#endif //PROD_PLAYCONTROLACT_H
