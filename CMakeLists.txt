cmake_minimum_required(VERSION 3.22.1)# 3.28.1)3.27.7

project(server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

find_package(Boost 1.71.0 COMPONENTS system REQUIRED) # 1.82.0
find_package(Threads REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(game source/game.cpp)
# add_executable(lobby source/server.cpp)

# Include directories
target_include_directories(game PRIVATE
    ${Boost_INCLUDE_DIRS}
    ${nlohmann_json_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(game PRIVATE
    Boost::system
    Threads::Threads
    nlohmann_json::nlohmann_json
)
