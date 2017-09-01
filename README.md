# dlib-android

[![Build Status](https://travis-ci.org/tzutalin/dlib-android.png)](https://travis-ci.org/tzutalin/dlib-android)

### Purpose
* Port [dlib](http://dlib.net/) to Android platform

* You can build it to dynamic or static library for Android. You can also build dlib's sample to Android executable file.

* This demonstrates dlib-android features, building with JNI.

### Grab the source

    $ git clone --recursive https://github.com/tzutalin/dlib-android.git
    $ cd dlib-android
    $ ./envsetup

### Prerequisites
* Download Android-NDK from [Android website](https://developer.android.com/ndk/downloads/index.html).

	 After downloading, go to the directory to which you downloaded the package to extract it

	 Export ANDROID_NDK_HOME in ~/.bashrc
     `$ vim ~/.bashrc`

	`export ANDROID_NDK_HOME=[NDK_PATH]/android-ndk-[version]`

    `export PATH=$PATH:$ANDROID_NDK_HOME`

* Install Android Debug Bride (ADB). You can download it via [Android SDK Manager](https://developer.android.com/sdk/installing/index.html) or $ sudo apt-get install android-tools-adb

* Prepare an Android device for test

### Build JNI code and shared library for Android application
* You can change the compiler architecture in dlib-android/jni/Application.mk

* Android Studio build JNI code and move to libs folder to use them, but if you don't compile the ndk run in command line to build and see warnings:

```sh
    $ cd [dlib-android]/app/src/main/jni
    $ ndk-build
```

### Run Android application
* Open Android Studio's project dlib-android to run face detection, face landmark, and so on
```

### Do you want to contribute
 * If you have any improvement or you've found any bug, send a pull request with the code.
 * Give me a star on this repository
 * <a href='https://ko-fi.com/A4263TV2' target='_blank'><img height='36' style='border:0px;height:36px;' src='https://az743702.vo.msecnd.net/cdn/kofi1.png?v=0' border='0' alt='Buy Me a Coffee at ko-fi.com' /></a>

### Future tasks
* Add more examples to [dlib-android-app](https://github.com/tzutalin/dlib-android-app)

