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

int main(int argc,char ** argv){

    TEST_INIReader();
    TEST_LOG();

    return 0;
}
