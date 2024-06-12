cmake_minimum_required(VERSION 3.20)
project(CompileFinalWork)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 尝试查找 Qt5，使用 QUIET 选项避免警告
find_package(Qt5 COMPONENTS Core Gui Widgets QUIET)
if (Qt5_FOUND)
    # 使用 Qt5
    set(Qt_VERSION_PREFIX "Qt5")
    message(STATUS "Using Qt5")
else ()
    # 尝试查找 Qt6
    find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
    if (Qt6_FOUND)
        # 使用 Qt6
        set(Qt_VERSION_PREFIX "Qt6")
        message(STATUS "Using Qt6")
    else ()
        message(FATAL_ERROR "Neither Qt6 nor Qt5 could be found")
    endif ()
endif ()

include_directories(include)

# 添加 src 目录及其子目录中的所有源文件
file(GLOB_RECURSE DIR_SRCS "src/*.cpp" "src/*.ui")
# 添加 src 目录及其子目录中的所有头文件
file(GLOB_RECURSE HEADER_FILES "src/*.h" "include/*.h")

add_executable(my_compiler
        main.cpp
        ${DIR_SRCS}
        ${HEADER_FILES}
)

target_link_libraries(my_compiler
        ${Qt_VERSION_PREFIX}::Core
        ${Qt_VERSION_PREFIX}::Widgets
        ${Qt_VERSION_PREFIX}::Gui
)
