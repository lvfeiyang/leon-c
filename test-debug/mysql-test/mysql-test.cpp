// https://github.com/mysql/mysql-connector-cpp

#include <mysqlx/xdevapi.h>

#include <iostream>

using ::std::cout;
using ::std::endl;
using namespace ::mysqlx;

int main(int argc, char *argv[]) {
  std::cout << "hello leon" << std::endl;

  // mysql -hrm-2zet8zu664682a0f5.mysql.rds.aliyuncs.com -umtreader -ppassw0rd mt_recommend
  const char *url = (argc > 1 ? argv[1] : "mysqlx://mtreader:passw0rd@tcp(rm-2zet8zu664682a0f5.mysql.rds.aliyuncs.com:3306)/mt_recommend");
  cout << "Creating session on " << url << " ..." << endl;
  Session sess(url);
  cout << "Session accepted, creating collection..." << endl;

  return 0;
}