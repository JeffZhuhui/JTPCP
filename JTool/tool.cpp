//
// Created by jeff on 23-8-24.
//

#include "tool.h"
#include <string>
#include <map>
#include <stdio.h>
#include <stdarg.h>
#include <algorithm>
#include "log.h"



namespace Comm {


    bool LogSet(const char* filepath, int level, bool quiet ){
        if(filepath){
            FILE* pFilePM = fopen(filepath, "w");
            if (NULL==pFilePM){
                printf("open file failed:%s\n",filepath);
                return false;
            } else {
                log_add_fp(pFilePM,level);
            }
        } else {
            log_set_level(level);
        }
        log_set_quiet(quiet);
        return true;
    }

/*
    int  Log(const char* szFmt, va_list args)
    {
        char szLog[MAX_MESSAGE_SIZE_CHARS] = { 0 };
        int nLog = vsnprintf(szLog, MAX_MESSAGE_SIZE_CHARS - 1, szFmt, args);
        printf("%s\n",szLog);
        return nLog;
    }
    int  Log(int lvl, const char *fmt, ...){
        int nLog = 0;
        if (lvl >= g_log_level)
        {
            va_list args;
            va_start(args, fmt);
            nLog =  Log(fmt, args);
            va_end(args);
        }
        return nLog;
    }*/

    bool SplitFilename(std::vector<std::string> & dest, const std::string & src){
        auto first = std::cbegin(src);
        std::string delims = "\\";
        while (first != std::cend(src))
        {
            const auto second = std::find_first_of(first, std::cend(src),
                                                   std::cbegin(delims), std::cend(delims));
            if (first != second)
                dest.emplace_back(first, second);
            if (second == std::cend(src))
                break;
            first = std::next(second);
        }
        return true;
    }
    bool UniteFilename(const std::vector<std::string> & src, std::string & dest){
        dest.clear();
        for(auto& it: src){
            if(!it.empty()){
                if(!dest.empty()){
                    dest += "\\";
                }
                dest += it;
            }
        }
        return true;
    }


}