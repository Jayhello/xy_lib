cmake_minimum_required(VERSION 3.9)

project(network_lib)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS -pthread)

# add sub dir files
file(GLOB COMMON_SRCS ./*/*.cpp ./*/*.h)

# for src/*
include_directories(../)

add_library(network_lib
        ${COMMON_SRCS})

target_link_libraries(
        network_lib
        comm_lib
)
