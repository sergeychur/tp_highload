cmake_minimum_required(VERSION 3.10)
project(static_server)

# -std=c++17
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

include_directories("include/")

set(SRC_LIST src/main.cpp src/config.cpp src/server.cpp src/routine.cpp src/http_handler.cpp include/http_handler.hpp)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} uv)