# dlib-android

[![Build Status](https://travis-ci.org/tzutalin/dlib-android.png)](https://travis-ci.org/tzutalin/dlib-android)

### Purpose
* Port [dlib](http://dlib.net/) to Andriod platform

* You can build it to dynamic or static library for Android. You can also build dlib's sample to Android executable file.

* You can refer to [dlib-android-app](https://github.com/tzutalin/dlib-android-app) which demonstrates dlib-android features

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

* [Optional] You can download a specific Android-OpenCV from http://opencv.org/downloads.html.  Download the version and copy the folders in native SDK to dlib-android/third_party/opencv. (Currently, itis 3.1)

* [Optional] You can use [NDK Download script](https://github.com/tzutalin/android-ndk-downloader) to specify the version of NDK to download

### Build and Run
* You can change the compiler architecture in dlib-android/jni/Application.mk

* The way to build native code and copy the shared library to Android Studio's project, dlib-android-app/dlib/src/main/jniLibs

```sh
    $ cd [dlib-android]
    $ python build.py
```

Alternative way to build native code and copy to the Android Studio's project manually:
```sh
    $ cd [dlib-android]
    $ ndk-build -j 2
    $ cp -r libs/* dlib-android-app/dlib/src/main/jniLibs
```

### Do you want to contribute
 * If you have any improvement or you've found any bug, send a pull request with the code.

 * Give me a star on this repository

### Future tasks
* Use cmake instead of Android.mk
* Add more examples to [dlib-android-app](https://github.com/tzutalin/dlib-android-app)

