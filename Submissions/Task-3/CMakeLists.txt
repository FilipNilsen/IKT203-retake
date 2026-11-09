cmake_minimum_required(VERSION 3.28)
project(execrise2)

set(CMAKE_CXX_STANDARD 17)

add_executable(execrise2 main.cpp)

add_custom_target(innlevering)
add_custom_command(TARGET innlevering PRE_BUILD COMMAND zip ARGS "-j" "innlevering.zip" "../main.cpp" "../CMakeLists.txt")