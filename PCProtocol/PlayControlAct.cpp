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
    if(act > int(PAction::EM_NOTIFY_FILES)){
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
        case PAction::EM_NOTIFY_PLAYED:
        case PAction::EM_ERROR:
        case PAction::EM_NOTIFY_FILES:{bparser = unmarshal(msg._data,pos,content);}break;
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
        case PAction::EM_NOTIFY_PLAYED:
        case PAction::EM_ERROR:
        case PAction::EM_NOTIFY_FILES:{bmarshal = marshal(msg._data,pos,pkg._content);}break;
        default:
            break;
    }
    msg._len = pos;
    if(!bmarshal){
        exc._error = "marshal package failed in content.";
    }
    return msg;
}


bool unmarshal(const std::vector<unsigned char >& src, unsigned int &pos, unsigned int & dest){
    if(src.size() < pos + 4){
        return false;
    }
    unsigned char v1 = src[pos++];
    unsigned char v2 = src[pos++];
    unsigned char v3 = src[pos++];
    unsigned char v4 = src[pos++];
    dest = v1 << 24 | v2 << 16 | v3 << 8 | v4;

    return true;
}
bool marshal(std::vector<unsigned char >& dest, unsigned int &pos, const unsigned int & src){
    if(dest.size() < pos + 4){
        return false;
    }
    dest[pos++] = src >> 24;
    dest[pos++] = src >> 16;
    dest[pos++] = src >> 8;
    dest[pos++] = src ;

    return true;
}

bool unmarshal(const std::vector<unsigned char >& src, unsigned int &pos, std::string & dest){
    unsigned int len = 0;
    bool bret = unmarshal(  src,  pos, len);
    if(!bret){
        return false;
    }
    if(src.size() < pos + len){
        return false;
    }
    dest = std::string ((char*)(&src[pos]), len);
    pos += len;
    return true;
}
bool marshal(std::vector<unsigned char >& dest, unsigned int &pos, const std::string & src){
    unsigned int len = src.length();
    bool bret = marshal(dest,pos,len);
    if(!bret){
        return false;
    }
    if(dest.size() < pos + len){
        return false;
    }
    for(unsigned int i = 0; i < len; ++i){
        dest[pos++] = src[i];
    }
    return true;
}