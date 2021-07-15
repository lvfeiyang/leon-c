#include <arrow/io/hdfs.h>
#include <arrow/status.h>
#include <arrow/result.h>

#include <iostream>

int main(int argc, char *argv[]) {
  std::cout << "hello leon" << std::endl;

  arrow::io::HdfsConnectionConfig conn_conf;
  conn_conf.host = "hdfs://inner-di-hdfs.1sapp.com";
  conn_conf.port = 8020;
  conn_conf.user = "algobase";
  conn_conf.extra_conf.insert({"dfs.client.socket-timeout", "120000"});
  std::shared_ptr<arrow::io::HadoopFileSystem> file_system;

  arrow::Status status =
      arrow::io::HadoopFileSystem::Connect(&conn_conf, &file_system);

  std::cout << "connect ret " << status.ok() << status.message() << std::endl;

  std::shared_ptr<arrow::io::HdfsReadableFile> hdfs_fd = nullptr;
  auto status1 = file_system->OpenReadable(
      "hdfs://inner-di-hdfs.1sapp.com:8020/innodata/user/tanhuabin/"
      "midu_jsb_online/assemble-ctr-v1c/serving/nn_donefile.txt",
      &hdfs_fd);

  std::cout << "open ret " << status1.ok() << status1.message() << std::endl;

  int64_t size = hdfs_fd->GetSize().ValueOrDie();
  std::string ret;
  ret.resize(size);
  int64_t pos = 0;
  const int64_t s_block_size = 4 * 1024 * 1024;
  while (pos < size) {
    int64_t read_size = std::min(s_block_size, size - pos);
    hdfs_fd->Read(read_size, (void *)(ret.data() + pos)).ValueOrDie();
    pos += read_size;
  }
  std::cout << "pos size " << (pos == size) << std::endl;
  status = hdfs_fd->Close();
  std::cout << status.ok() << status.message() << std::endl;

  // std::cout << ret << std::endl;
  return 0;
}
