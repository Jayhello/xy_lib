cmake_minimum_required(VERSION 3.10)
project(xy_lib)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")

include_directories(./)

#set(BOOST_ROOT "/mnt/f/3rd/dependence/")
#set(BOOST_INCLUDE_DIRS "${BOOST_ROOT}/include")
#set(BOOST_LIBRARY_DIRS "${BOOST_ROOT}/lib")

find_package(Protobuf REQUIRED)
message("main Protobuf lib: ${PROTOBUF_LIBRARIES}, inc: ${PROTOBUF_INCLUDE_DIR}")

add_subdirectory(src/comm)

add_subdirectory(src/network)

add_subdirectory(src/common_pb)

# for common.pb.h
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src)
message("include current bin_dir: ${CMAKE_CURRENT_BINARY_DIR}")

add_subdirectory(example/util)
add_subdirectory(example/concurrency)
add_subdirectory(example/net)
add_subdirectory(example/codec)

#add_subdirectory(example/demo_http)
#add_subdirectory(example/demo_raw_network)

#set(SOURCE_FILES main.cpp)
#add_executable(my_project ${SOURCE_FILES})
#target_link_libraries(my_project common_lib)