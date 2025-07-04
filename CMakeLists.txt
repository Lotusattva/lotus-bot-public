cmake_minimum_required(VERSION 3.28)

project(lotus_bot LANGUAGES CXX VERSION 0.1.0 DESCRIPTION
    "A discord bot written in C++ using D++ library"
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/calculator/calculator.cpp
    src/calculator/calculator_interface.cpp
    src/calculator/calculator_interface_control_flow.cpp
    src/feet.cpp
    src/poll.cpp
    src/scheduler.cpp
)

find_package(DPP REQUIRED)

target_link_libraries(${PROJECT_NAME} ${DPP_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${DPP_INCLUDE_DIRS})