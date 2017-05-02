cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-std=c++11")

project(Server_Sea_Battle)
add_executable(${PROJECT_NAME} "main.cpp" "server.hpp" "server.cpp")
target_link_libraries(${PROJECT_NAME} boost_system)
