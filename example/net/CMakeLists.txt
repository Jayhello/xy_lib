cmake_minimum_required(VERSION 3.9)

project(demo_net)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS -pthread)

include_directories(../../src)

#set(BOOST_ROOT "/mnt/f/3rd/dependence/")
#set(BOOST_INCLUDE_DIRS "${BOOST_ROOT}/include")
#set(BOOST_LIBRARY_DIRS "${BOOST_ROOT}/lib")


#find_package(Boost REQUIRED regex date_time thread system filesystem program_options)
#include_directories(${BOOST_INCLUDE_DIRS})
#message("boost lib: ${Boost_LIBRARIES}")
#message("boost inc:${Boost_INCLUDE_DIR}")

add_executable(demo_net demo_net.cpp)
target_link_libraries(demo_net
        comm_lib
        network_lib
)

add_executable(demo_server demo_server.cpp)
target_link_libraries(demo_server
        comm_lib
        network_lib
)

add_executable(demo_client demo_client.cpp)
target_link_libraries(demo_client
        comm_lib
        network_lib
        )

#add_executable(demo_rwlock demo_rwlock.cpp)
#target_link_libraries(demo_rwlock
#        common_lib
#        ${Boost_LIBRARIES}
#        )

#add_executable(demo_comm demo_comm.cpp)
#target_link_libraries(demo_comm
#        common_lib
#        ${Boost_LIBRARIES}
#        )

#find_package(PkgConfig)
#pkg_check_modules(YAMLCPP REQUIRED yaml-cpp>=0.5)
#include_directories(${YAML_CPP_INCLUDE_DIR})
#message("yaml cpp: ${YAML_CPP_INCLUDE_DIR}, lib: ${YAML_CPP_LIBRARIES} ${YAMLCPP_LIBRARIES}")

#add_executable(demo_yaml demo_yaml.cpp)
#target_link_libraries(demo_yaml
#        common_lib
#        ${YAMLCPP_LIBRARIES}
#        ${Boost_LIBRARIES})
