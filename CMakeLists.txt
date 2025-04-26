cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(MSVC)
  add_definitions(-D_WIN32_WINNT=0x600)
endif()

project(try4 VERSION 0.1 LANGUAGES CXX)
set (CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(FETCHCONTENT_QUIET OFF)
set(ABSL_ENABLE_INSTALL ON) 

find_package(Threads REQUIRED)
# ref: https://github.com/grpc/grpc/blob/master/examples/cpp/cmake/common.cmake#L70

# Another way is to use CMake's FetchContent module to clone gRPC at
# configure time. This makes gRPC's source code available to your project,
# similar to a git submodule.
message(STATUS "Using gRPC via add_subdirectory (FetchContent).")
include(FetchContent)
FetchContent_Declare(grpc GIT_REPOSITORY https://github.com/grpc/grpc.git GIT_TAG v1.72.0)
FetchContent_MakeAvailable(grpc)

# Since FetchContent uses add_subdirectory under the hood, we can use
# the grpc targets directly from this build.
set(_PROTOBUF_LIBPROTOBUF libprotobuf)
set(_REFLECTION grpc++_reflection)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protoc>)
set(_GRPC_GRPCPP grpc++)
if(CMAKE_CROSSCOMPILING)
    find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else()
    set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:grpc_cpp_plugin>)
endif()


add_subdirectory(proto)  
add_subdirectory(server)
add_subdirectory(client)