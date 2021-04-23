cmake_minimum_required(VERSION 3.19)
project(client)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp)

include_directories(${converter_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} converter)
