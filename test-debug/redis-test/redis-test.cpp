// https://github.com/redis/hiredis/blob/master/examples/example.c

#include <hiredis/hiredis.h>

#include <iostream>

int main(int argc, char* argv[]) {
  std::cout << "hello leon" << std::endl;

  redisContext* c;
  redisReply* reply;
  struct timeval timeout = {1, 500000};
  c = redisConnectWithTimeout("127.0.0.1", 6379, timeout);
  reply = (redisReply*)redisCommand(c, "AUTH %s", "test");
  freeReplyObject(reply);
  reply = (redisReply*)redisCommand(c, "GET leon");
  std::cout << (reply == NULL) << reply->str << std::endl;
  freeReplyObject(reply);
  std::cout << (c == NULL) << c->errstr << std::endl;
  redisFree(c);

  return 0;
}
