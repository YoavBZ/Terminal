cmake_minimum_required(VERSION 3.8)
project(Assignment1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES "Commands - Tests.cpp" src/Files.cpp src/FileSystem.cpp src/Environment.cpp src/Commands.cpp)
#set(SOURCE_FILES src/Main.cpp src/Files.cpp src/FileSystem.cpp src/Environment.cpp src/Commands.cpp)
add_executable(Assignment1 ${SOURCE_FILES})