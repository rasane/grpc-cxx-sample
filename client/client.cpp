#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "example.grpc.pb.h" // Generated from example.proto

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using example::ExampleService;
using example::HelloRequest;
using example::HelloReply;

class ExampleClient {
public:
    ExampleClient(std::shared_ptr<Channel> channel)
        : stub_(ExampleService::NewStub(channel)) {}

    // Method to call the SayHello RPC
    std::string SayHello(const std::string& name) {
        // Prepare the request
        HelloRequest request;
        request.set_name(name);

        // Prepare the reply
        HelloReply reply;

        // Context for the client
        ClientContext context;

        // Call the RPC
        Status status = stub_->SayHello(&context, request, &reply);

        // Handle the response
        if (status.ok()) {
            return reply.message();
        } else {
            std::cerr << "RPC failed: " << status.error_message() << std::endl;
            return "RPC failed";
        }
    }

private:
    std::unique_ptr<ExampleService::Stub> stub_;
};
std::string GetTempPathFromEnv() {
    char tempPath[MAX_PATH];
    DWORD length = GetEnvironmentVariable(TEXT("TEMP"), tempPath, MAX_PATH);
    if (length == 0 || length > MAX_PATH) {
        throw std::runtime_error("Failed to get TEMP environment variable");
    }
    return std::string(tempPath);
  }

int main(int argc, char** argv) {
    // Create a channel to connect to the server
    std::string temp_path = GetTempPathFromEnv() ; // Get the temp path from the environment variable
    std::string target_address = "unix:" + temp_path + "/grpc_named_pipe";

    ExampleClient client(grpc::CreateChannel(target_address, grpc::InsecureChannelCredentials()));

    // Get the name from the command line or use a default
    std::string name = "world";
    if (argc > 1) {
        name = argv[1];
    }

    // Call the SayHello RPC
    std::string reply = client.SayHello(name);
    std::cout << "Server replied: " << reply << std::endl;

    return 0;
}