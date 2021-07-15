#include <sys/stat.h>

// #include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>
#include <atomic>

class Base {
 public:
  virtual void fun() { std::cout << "Base::fun()" << std::endl; }
};

class Derived : public Base {
 public:
  void fun() override { std::cout << "Derived::fun()" << std::endl; }
};

void quickSort(std::vector<int64_t>& arr, int lo, int hi) {
  if (lo < hi) {
    // std::cout << lo << " " << hi << std::endl;
    int64_t midVal = arr[lo], tmpVal;
    int i = lo + 1, j = hi;
    while (true) {
      while (i <= hi && arr[i] < midVal) {
        i++;
      }
      while (j >= lo + 1 && arr[j] > midVal) {
        j--;
      }
      if (i > j) {
        break;
      }
      tmpVal = arr[i];
      arr[i] = arr[j];
      arr[j] = tmpVal;
      i++, j--;
    }
    tmpVal = arr[lo];
    arr[lo] = arr[j];
    arr[j] = tmpVal;

    quickSort(arr, lo, j - 1);
    quickSort(arr, j + 1, hi);
  }
}

int main(int argc, char* argv[]) {
  std::cout << "hello leon" << std::endl;

  std::atomic<bool> ready_(false);
  std::cout << "leon_debug " << ready_ << std::endl;

  return 0;
  /*
    std::vector<int64_t> sortArr{0, 1, 1, 5, 2, 7, 3, 11, 56, 5, 12, 56};
    quickSort(sortArr, 0, sortArr.size() - 1);
    std::ostringstream oss;
    for (int64_t a : sortArr) {
      oss << a << ", ";
    }
    std::cout << oss.str() << std::endl;

    return 0;

    Base* base = new Derived;
    base->fun();

    return 0;

    std::ofstream ofs;
    ofs.open("test.txt", std::ofstream::out | std::ofstream::trunc);
    std::string data("124");
    ofs << data.data();
    ofs.flush();
    ofs << "567";
    // ofs.clear();
    ofs.close();

    // std::filesystem::create_directory("testDir/1");

    return 0;
  */
}