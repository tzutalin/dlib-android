#include <glog/logging.h>

int main() {
    google::InitGoogleLogging("Test");
    LOG(INFO) << "Dump log test";
    return 0;
}
