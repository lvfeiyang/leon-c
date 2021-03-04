#include <folly/Conv.h>
#include <folly/FBString.h>
#include <folly/io/IOBuf.h>
#include <gtest/gtest.h>

#include "service/Msg.h"
#include "util/pbdata/serv-msg.pb.h"

TEST(module_service, test_endian) {
  /*
auto buf = folly::IOBuf::create(1000);
uint64_t id = folly::Endian::big(folly::to<uint64_t>(44789));
memcpy(buf->writableData(), (uint8_t*)&id, 8);
buf->append(8);

FsService::PushMsg msg(buf->length(), buf->data());
std::cout << msg.show() << std::endl;
  */

  ServMsg::PushReq a;
  auto oneInfo = a.add_infos();
  oneInfo->set_key(11);
  oneInfo->set_data("test");
  oneInfo = a.add_infos();
  oneInfo->set_key(22);
  oneInfo->set_data("test2");
  std::string b;
  // folly::fbstring b;
  a.SerializeToString(&b);
  auto msg = FsService::InOutMsg(b.length() + 1, (uint8_t*)b.c_str(),
                                 FsService::InOutMsg::MsgType::Push);
  std::cout << msg.show() << std::endl;

  ServMsg::PullReq a1;
  a1.add_keys(33);
  a1.add_keys(44);
  a1.add_field(1);
  a1.add_field(2);
  a1.SerializeToString(&b);
  msg = FsService::InOutMsg(b.length() + 1, (uint8_t*)b.c_str(),
                            FsService::InOutMsg::MsgType::Pull);
  std::cout << msg.show() << std::endl;
  auto keys = msg.getPullIds();
  folly::fbstring s;
  for (auto v : keys) {
    s += folly::to<folly::fbstring>(v) + ",";
  }
  std::cout << s << std::endl;

  msg.setPullData(0, "test");
  msg.setPullData(1, "test1");
  std::string rspS = msg.EncodeRsp(1);
  std::cout << rspS.size() << ", " << rspS << std::endl;

}  // EXPECT_EQ

// int main(int argc, char **argv) {
//   ::testing::InitGoogleTest(&argc, argv);
// 	gflags::ParseCommandLineFlags(&argc, &argv, true);
//   google::InitGoogleLogging(argv[0]);
//   google::InstallFailureSignalHandler();

//   return RUN_ALL_TESTS();
// }