//
// Created by jeff on 23-8-24.
//

#ifndef DAEFORORACLE_TOOL_H
#define DAEFORORACLE_TOOL_H

#include <string>
#include <map>
#include <vector>



namespace Comm {

    bool LogSet(const char* filepath, int level, bool quiet );

    bool SplitFilename(std::vector<std::string> & dest, const std::string & src);
    bool UniteFilename(const std::vector<std::string> & src, std::string & dest);

}


#endif //DAEFORORACLE_TOOL_H
