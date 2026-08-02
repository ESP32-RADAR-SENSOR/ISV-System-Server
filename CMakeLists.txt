cmake_minimum_required(VERSION 3.20)

project(ISVSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED COMPONENTS system)


add_executable(isv_server
    apps/server_main.cpp
)

add_executable(sensor_simulator
    apps/simulator_main.cpp
    src/SensorMessage.cpp
)

target_include_directories(isv_server
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(sensor_simulator
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(isv_server
    PRIVATE
        Boost::system
        nlohmann_json::nlohmann_json
)

target_link_libraries(sensor_simulator
    PRIVATE
        Boost::system
        nlohmann_json::nlohmann_json
)