#include <iostream>

#include "pbdata/test.pb.h"

int main(int argc, char* argv[]) {
  std::cout << "hello leon" << std::endl;

  test::TestPb a, c;
  a.set_page(1);

  std::string b;
  a.SerializeToString(&b);
  c.ParseFromString(b);

  std::cout << a.DebugString() << b.capacity() << c.DebugString() << std::endl;
  return 0;
}
