cmake_minimum_required(VERSION 3.20)
project(CompileFinalWork)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)

aux_source_directory(src DIR_SRCS)

add_executable(my_compiler
        main.cpp
        ${DIR_SRCS}
)
