
cmake_minimum_required (VERSION 3.8)
project (JTPCP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)
set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_FIND_USE_SYSTEM_ENVIRONMENT_PATH FALSE)

add_compile_options("-g")	# Allow output core dump

MESSAGE(CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH $ORIGIN $ORIGIN/lib)

if(NOT DEFINED OUT_PUT_SO)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ARCH_BIT x64)
    else()
        set(ARCH_BIT x86)
    endif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(SUBDIR_NAME ${CMAKE_BUILD_TYPE}_${ARCH_BIT}) # LINK_DIR_NAME VARIANT_NAME FolderName
    set(OUT_PUT_SO ${CMAKE_CURRENT_SOURCE_DIR}/bin/${SUBDIR_NAME})
    file(MAKE_DIRECTORY ${OUT_PUT_SO})
endif(NOT DEFINED OUT_PUT_SO)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUT_PUT_SO})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUT_PUT_SO})
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${OUT_PUT_SO})   # output lib file
else()
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${OUT_PUT_SO})   # output lib file
endif()
link_directories(${OUT_PUT_SO})
MESSAGE(OUT_PUT_SO=${OUT_PUT_SO})

if (UNIX)
    set(CMAKE_CXX_FLAGS "-fPIC -Wall -pedantic -ldl ${CMAKE_CXX_FLAGS}")
else()
    set(CMAKE_CXX_FLAGS "-lwsock32 ${CMAKE_CXX_FLAGS}")

ENDIF()

add_subdirectory(JClient)
#add_subdirectory(JServer)
#add_subdirectory(JWinServer)
add_subdirectory(JTool)
add_subdirectory(PCProtocol)
add_subdirectory(Test)