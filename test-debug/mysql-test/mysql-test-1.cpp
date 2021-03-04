// https://dev.mysql.com/doc/dev/connector-cpp/8.0/jdbc_ref.html

#include <mysql/jdbc.h>

#include <iostream>

int main(int argc, char *argv[]) {
  std::cout << "hello leon" << std::endl;

  const char *url = "tcp://rm-2zet8zu664682a0f5.mysql.rds.aliyuncs.com";

  sql::Driver *driver = sql::mysql::get_driver_instance();

  /* Using the Driver to create a connection */
  std::cout << "Creating session on " << url << " ..." << std::endl
            << std::endl;
  boost::scoped_ptr<sql::Connection> con(
      driver->connect(url, "mtreader", "passw0rd"));

  con->setSchema("mt_recommend");
  boost::scoped_ptr<sql::Statement> stmt(con->createStatement());

  boost::scoped_ptr<sql::ResultSet> res(
      // stmt->executeQuery("SELECT 'Welcome to Connector/C++' AS _message"));
      stmt->executeQuery("select * from t_test limit 2"));

  while (res->next()) {
    std::cout << "\t... MySQL replies: " << res->getString("id") << std::endl;
    std::cout << "\t... say it again, MySQL" << std::endl;
    std::cout << "\t....MySQL replies: " << res->getString(1) << std::endl;
  }

  std::cout << "isValid: " << con->isValid() << std::endl;

  return 0;
}