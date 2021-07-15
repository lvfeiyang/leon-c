#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
  std::cout << "hello leon" << std::endl;
  std::vector<float> a;
  a.resize(10);
  for (size_t i = 0; i < 10; i++) {
    std::cout << a[i] << std::endl;
  }
  return 0;
}