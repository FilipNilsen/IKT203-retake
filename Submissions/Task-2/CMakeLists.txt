cmake_minimum_required(VERSION 3.28)
project(exercise1)

set(CMAKE_CXX_STANDARD 23)

add_executable(exercise1 main.cpp)

# add_custom_target(innlevering)
# add_custom_command(TARGET innlevering COMMAND zip ARGS "-j" "innlevering.zip" "../main.cpp" "../CMakeLists.txt")
