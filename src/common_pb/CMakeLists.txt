project(proto_lib)
cmake_minimum_required (VERSION 3.9)

# 这里暂时不好指定输出目录, 因此在 xy_lib\cmake-build-debug\src\common_pb\common.pb.h 这里将生成的文件临时复制过来
protobuf_generate_cpp(PROTO_SRC PROTO_HEADER
#        PROTOC_OUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}
        common.proto busi_pb.proto)

# PROTO_SRCS - 存储.pb.cc文件的变量名称
# PROTO_HDRS- 存储.pb.h文件的变量名称

add_library(proto_lib
        ${PROTO_HEADER}
        ${PROTO_SRC}
        )
