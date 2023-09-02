
project (jclient)

include_directories(../JTool)
include_directories(../PCProtocol)


if (UNIX)
    add_executable(${PROJECT_NAME} main.cpp)
else()
    add_executable(${PROJECT_NAME} main_win.cpp)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
ENDIF()


target_link_libraries(${PROJECT_NAME} PRIVATE jtool)
target_link_libraries(${PROJECT_NAME} PRIVATE pcprotocol)