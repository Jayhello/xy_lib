cmake_minimum_required(VERSION 3.9)

project(demo_concurrency)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(demo_codec demo_codec.cpp)
target_link_libraries(demo_codec
        comm_lib
        proto_lib
        network_lib
        ${PROTOBUF_LIBRARIES}
        )
