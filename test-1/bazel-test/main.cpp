#include <iostream>

int main(int argc, char* argv[]) {
  std::cout << "hello leon" << std::endl;
  folly::string a = "test";
  std::cout << "hello " << a << std::endl;
  std::vector<float> a;
  a.resize(10);
  for (size_t i = 0; i < 10; i++) {
    std::cout << i << std::endl;
  }
}
