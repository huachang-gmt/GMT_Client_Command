cmake_minimum_required(VERSION 3.16)

project(GMT_Client_Command
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(GMT_Client_Command
    src/main.cpp
)