
project (jtool)

add_library(${PROJECT_NAME} STATIC
        ini.cpp
        INIReader.cpp
        log.cpp
        tool.cpp)