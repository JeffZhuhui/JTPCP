#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#include "log.h"
#include "tool.h"
#include "PlayControlAct.h"
#include "INIReader.h"

void TEST_INIReader(){
    INIReader cfg("C:\\temp\\config.ini");
    if (cfg.ParseError() < 0) {
        return  ;
    }
    std::string path = cfg.GetString("VIDEO","videoPath","");
}

void TEST_LOG(){
    Comm::LogSet("C:\\temp\\test.log", LOG_INFO, false);
    DAElog_trace("TEST LOG: %s", "123");
    DAElog_warn("TEST LOG: %s", "123");
}


void TEST_tool(){
    std::string src = "\\123.mp4\\234.mp4\\\\345.mp4";
    std::vector<std::string> allfiles;
    Comm::SplitFilename(allfiles,src);

    std::string dest;
    Comm::UniteFilename(allfiles,dest);
    if(dest == "123.mp4\\234.mp4\\345.mp4"){
        printf("succeed.\n");
    } else {
        printf(" TEST_tool failed.\n");
    }
}



int main(int argc,char ** argv){


    PCPackage pkg;
    pkg._pact = PAction::EM_NOTIFY_PLAYED;
    pkg._content = "meeting1.mp4";


    Execption exc;
    PCMsg  msg= PlayControlAct::Marshal(pkg,exc);

    msg._data[3] = 12;
    PCPackage pkg2 = PlayControlAct::Unmarshal(msg, exc);

    TEST_tool();

    TEST_INIReader();
    TEST_LOG();

    return 0;
}
