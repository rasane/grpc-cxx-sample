
# Generate gRPC and protobuf files
set(PROTO_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PROTO_GEN_DIR ${PROTO_SRC_DIR}/generated)

file(MAKE_DIRECTORY ${PROTO_GEN_DIR})

set(PROTO_FILES ${PROTO_SRC_DIR}/example.proto)
set(PROTO_SRCS ${PROTO_GEN_DIR}/example.pb.cc)
set(PROTO_HDRS ${PROTO_GEN_DIR}/example.pb.h)
set(GRPC_SRCS ${PROTO_GEN_DIR}/example.grpc.pb.cc)
set(GRPC_HDRS ${PROTO_GEN_DIR}/example.grpc.pb.h)
set(PROTO_GEN_FILES ${PROTO_SRCS} ${PROTO_HDRS} ${GRPC_SRCS} ${GRPC_HDRS})
set(PROTO_BINARY_DIR "${PROTO_SRC_DIR}/generated")

add_custom_command(
      OUTPUT ${PROTO_GEN_FILES}
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${PROTO_BINARY_DIR}"
        --cpp_out "${PROTO_BINARY_DIR}"
        -I "${PROTO_SRC_DIR}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${PROTO_FILES}"
      DEPENDS "${PROTO_FILES}")

message(STATUS "YAY this command worked")

add_library(proto_lib ${PROTO_SRCS} ${PROTO_HDRS} ${GRPC_SRCS} ${GRPC_HDRS})
target_include_directories(proto_lib PUBLIC ${PROTO_GEN_DIR})
target_link_libraries(proto_lib PUBLIC 
${_REFLECTION}
${_GRPC_GRPCPP}
${_PROTOBUF_LIBPROTOBUF})

# Export the include directory for other targets
set(PROTO_INCLUDE_DIRS ${PROTO_GEN_DIR} PARENT_SCOPE)