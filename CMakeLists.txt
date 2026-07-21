cmake_minimum_required(VERSION 3.20)

project(ISVSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(isv_system
    src/main.cpp
    src/SensorMessage.cpp
)

target_include_directories(isv_system
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)