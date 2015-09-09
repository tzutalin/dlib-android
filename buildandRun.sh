#!/bin/sh
ndk-build
# Push and run
# svm_ex
adb push libs/armeabi-v7a/svm_ex /data/local/tmp
adb shell ./data/local/tmp/svm_ex

# max_cost_assignment_ex
adb push libs/armeabi-v7a/max_cost_assignment_ex /data/local/tmp
adb shell ./data/local/tmp/max_cost_assignment_ex

# svr_ex
adb push libs/armeabi-v7a/svr_ex /data/local/tmp
adb shell ./data/local/tmp/svr_ex

# find object samples
adb push jni/other_ex/data/lena.jpg /sdcard/lena.jpg
adb push libs/armeabi-v7a/TestSelectiveSearch /data/local/tmp
adb shell ./data/local/tmp/TestSelectiveSearch /sdcard/lena.jpg
# face_lardmark_ex, need to push shape_predictor_68_face_landmarks.dat to mobile devies
adb push libs/armeabi-v7a/face_landmark /data/local/tmp
