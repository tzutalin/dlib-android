#!/bin/sh
ndk-build clean
rm -rf libs
rm -rf obj
adb shell rm ./data/local/tmp/svm_ex
adb shell rm ./data/local/tmp/max_cost_assignment_ex
adb shell rm ./data/local/tmp/svr_ex
adb shell rm ./data/local/tmp/TestSelectiveSearch
adb shell rm ./data/local/tmp/face_landmark
