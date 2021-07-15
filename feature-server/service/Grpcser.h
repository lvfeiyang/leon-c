#pragma once
#include "util/pbdata/serv-msg.grpc.pb.h"
namespace FsService {

class PullerServiceImpl final : public ServMsg::Puller::Service {
  grpc::Status Pull(grpc::ServerContext* context,
                    const ServMsg::PullReq* request,
                    ServMsg::PullRsp* reply) override {
    return grpc::Status::OK;
  }
};

}  // namespace FsService