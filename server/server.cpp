


#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "example.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using example::ExampleService;
using example::HelloRequest;
using example::HelloReply;


#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#include <unistd.h>
#endif


class ExampleServiceImpl final : public ExampleService::Service {
  Status SayHello(ServerContext* context, const HelloRequest* request, HelloReply* reply) override {
    std::string prefix("Hello ");
    std::cout << "Server received  " << request->name() << std::endl;

    reply->set_message(prefix + request->name());
    return Status::OK;
  }
};

std::string GetTempPathFromEnv() {
#if defined(WIN32)
    char tempPath[MAX_PATH];
    DWORD length = GetEnvironmentVariable(TEXT("TEMP"), tempPath, MAX_PATH);
    if (length == 0 || length > MAX_PATH) {
        throw std::runtime_error("Failed to get TEMP environment variable");
    }
    return std::string(tempPath);
#else
    const char* tempPath = std::getenv("TMPDIR");
    if (!tempPath) {
        tempPath = "/tmp"; // Default to /tmp if TMPDIR is not set
    }
    return std::string(tempPath);
#endif
}

void RunServer() {
  std::string temp_path = GetTempPathFromEnv() ;
  std::string server_address = "unix:" + temp_path + "/grpc_named_pipe";
  ExampleServiceImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
}