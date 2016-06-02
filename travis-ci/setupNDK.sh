#!/usr/bin/env sh
wget https://dl.google.com/android/ndk/android-ndk-r10e-linux-x86_64.bin
chmod +x android-ndk-r10e-linux-x86_64.bin
./android-ndk-r10e-linux-x86_64.bin 2>/dev/null
mv android-ndk-r10e android-ndk
