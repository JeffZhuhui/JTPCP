//
// Created by jhzhu on 9/1/2023.
//

#ifndef PROD_PLAYCONTROLACT_H
#define PROD_PLAYCONTROLACT_H
#include <string>
#include <vector>

enum class PAction{
    EM_UNKNOWN,
    EM_PLAY = 1,
    EM_PAUSE,
    EM_VOL_ADD,
    EM_VOL_SUB,
    EM_VOL_SET,
    EM_REPLAY,
    EM_STOP,
    EM_SELECT,
    EM_NOTIFY,
    EM_NOTIFY_PLAYED,
    EM_ERROR
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
    static PCPackage Unmarshal(const PCMsg& msg, Execption & exc);
    static PCMsg Marshal(const PCPackage & pkg, Execption & exc);
};


#endif //PROD_PLAYCONTROLACT_H
