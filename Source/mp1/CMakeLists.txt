project(cptrab2)
cmake_minimum_required(VERSION 3.5)
#aux_source_directory(. SRC_LIST)

set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_C_STANDARD 11)
#list(APPEND CMAKE_C_FLAGS "-Wall -Wextra -pedantic")

file(GLOB cptrab_H
    "*.h"
)

file(GLOB cptrab_SRC
    "*.c"
)

#include_directories(cptrab ${PROJECT_SOURCE_DIR})

#add_executable(${PROJECT_NAME} ${SRC_LIST})
add_executable(cptrab_bin ${cptrab_H} ${cptrab_SRC})

target_link_libraries(cptrab_bin pthread profiler)
