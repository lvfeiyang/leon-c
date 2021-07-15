
#include <grpcpp/grpcpp.h>

#include <iostream>

#include "pbdata/hello.grpc.pb.h"

class GreeterClient {
 public:
  GreeterClient(std::shared_ptr<grpc::Channel> channel)
      : stub_(hello::Greeter::NewStub(channel)) {}

  std::string SayHello(const std::string& user) {
    hello::HelloRequest request;
    request.set_name(user);
    hello::HelloReply reply;

    grpc::ClientContext context;

    // The actual RPC.
    grpc::Status status = stub_->SayHello(&context, request, &reply);

    if (status.ok()) {
      return reply.message();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

 private:
  std::unique_ptr<hello::Greeter::Stub> stub_;
};

int main(int argc, char* argv[]) {
  std::cout << "hello leon" << std::endl;

  GreeterClient greeter(grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials()));
  std::string user("world");
  std::string reply = greeter.SayHello(user);
  std::cout << "Greeter received: " << reply << std::endl;

  return 0;
}