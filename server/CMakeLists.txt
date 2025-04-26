add_executable(server server.cpp)
target_include_directories(server PRIVATE ${PROTO_INCLUDE_DIRS})
# target_link_libraries(server PRIVATE proto_lib gRPC::grpc++_reflection)

target_link_libraries(server PRIVATE proto_lib ${_REFLECTION}
${_GRPC_GRPCPP}
${_PROTOBUF_LIBPROTOBUF})

