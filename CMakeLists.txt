cmake_minimum_required(VERSION 3.0.0)
project(Window-Window VERSION 0.1.0)

file(GLOB_RECURSE SOURCE_FILES
    "src/*.c"
    "src/*.h"
)

foreach(name ${SOURCE_FILES})
    message(STATUS "${name}")
endforeach()

include_directories(src/engine)

add_executable(Window-Window ${SOURCE_FILES})
