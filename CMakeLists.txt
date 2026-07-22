cmake_minimum_required(VERSION 3.20)

project(ISVSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(nlohmann_json CONFIG REQUIRED)

add_executable(isv_system
    src/main.cpp
    src/SensorMessage.cpp
)

target_include_directories(isv_system
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(isv_system
    PRIVATE
        nlohmann_json::nlohmann_json
)