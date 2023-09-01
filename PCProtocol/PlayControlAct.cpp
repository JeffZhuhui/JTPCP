//
// Created by jhzhu on 9/1/2023.
//

#include "PlayControlAct.h"

bool unmarshal(const std::vector<unsigned char >& src, unsigned int &pos, unsigned int & dest);
bool marshal(std::vector<unsigned char >& dest, unsigned int &pos, const unsigned int & src);

bool unmarshal(const std::vector<unsigned char >& src, unsigned int &pos, std::string & dest);
bool marshal(std::vector<unsigned char >& dest, unsigned int &pos, const std::string & src);

PCPackage PlayControlAct::Unmarshal(const PCMsg& msg, Execption & exc){
    PCPackage pkg;
    unsigned int act = 0;
    unsigned int pos = 0;
    bool bparser = unmarshal(msg._data,pos,act);
    if(!bparser){
        exc._error = "invalid tcp package length";
        return pkg;
    }
    if(act > int(PAction::EM_ERROR)){
        exc._error = "invalid command";
        return pkg;
    }
    pkg._pact = PAction(act);
    std::string content;
    switch (PAction(act)) {
        case PAction::EM_UNKNOWN:{}break;
        case PAction::EM_PLAY :{}break;
        case PAction::EM_PAUSE:{}break;
        case PAction::EM_VOL_ADD:{}break;
        case PAction::EM_VOL_SUB:{}break;
        case PAction::EM_VOL_SET:{bparser = unmarshal(msg._data,pos,content);}break;
        case PAction::EM_REPLAY:{}break;
        case PAction::EM_STOP:{}break;
        case PAction::EM_SELECT:{bparser = unmarshal(msg._data,pos,content);}break;
        case PAction::EM_NOTIFY:{}break;
        case PAction::EM_NOTIFY_PLAYED:{bparser = unmarshal(msg._data,pos,content);}break;
        case PAction::EM_ERROR:{bparser = unmarshal(msg._data,pos,content);}break;
        default:
            break;
    }
    if(!bparser){
        exc._error = "invalid tcp package length,parser content error.";
        return pkg;
    }
    pkg._content = content;
    pkg._packlen = pos;
    return pkg;
}

PCMsg PlayControlAct::Marshal(const PCPackage & pkg, Execption & exc){
    PCMsg msg;
    unsigned int pos = 0;
    bool bmarshal = marshal(msg._data,pos,(unsigned int)pkg._pact);
    if(!bmarshal){
        exc._error = "marshal package failed in command.";
        return msg;
    }
    switch (pkg._pact) {
        case PAction::EM_UNKNOWN:{}break;
        case PAction::EM_PLAY :{}break;
        case PAction::EM_PAUSE:{}break;
        case PAction::EM_VOL_ADD:{}break;
        case PAction::EM_VOL_SUB:{}break;
        case PAction::EM_VOL_SET:{bmarshal = marshal(msg._data,pos,pkg._content);}break;
        case PAction::EM_REPLAY:{}break;
        case PAction::EM_STOP:{}break;
        case PAction::EM_SELECT:{bmarshal = marshal(msg._data,pos,pkg._content);}break;
        case PAction::EM_NOTIFY:{}break;
        case PAction::EM_NOTIFY_PLAYED:{bmarshal = marshal(msg._data,pos,pkg._content);}break;
        case PAction::EM_ERROR:{bmarshal = marshal(msg._data,pos,pkg._content);}break;
        default:
            break;
    }
    msg._len = pos;
    if(!bmarshal){
        exc._error = "marshal package failed in content.";
    }
    return msg;
}