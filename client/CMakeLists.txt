add_executable(client client.cpp)

target_include_directories(client PRIVATE ${PROTO_INCLUDE_DIRS})

target_link_libraries(client PRIVATE proto_lib 
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
