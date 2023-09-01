
project (test)

include_directories(../JTool)
include_directories(../PCProtocol)



add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE jtool)
target_link_libraries(${PROJECT_NAME} PRIVATE pcprotocol)