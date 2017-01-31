#include <jni.h>
#include <glog/logging.h>

#pragma GCC diagnostic ignored "-Wwrite-strings"
int main() {
  google::InitGoogleLogging("example");
  LOG(INFO) << "Dump log test";
  return 0;
}
