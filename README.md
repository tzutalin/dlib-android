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

### Build JNI code and shared library for Android application
* You can change the compiler architecture in dlib-android/jni/Application.mk

* The way to build the shared library for Android application

```sh
    $ cd [dlib-android]
    $ python build.py
```

Alternative way to build native code and copy to the Android Studio's project manually:
```sh
    $ cd [dlib-android]
    $ ndk-build -j 2
    $ cp -r libs/* androidstudio-examples/dlib-android-app/dlib/src/main/jniLibs
```

### Run Android application
* Open Android Studio's projects in androidstudio-examples/dlib-android-app to run face detection, face landmark, and so on

### Folder structure

```
├── data                    # Test data or the models for detection and landmarks
├── dlib                    # Source files of dlib. It is a submodule
├── jni                     # Source files of JNI codes and their make files
├── androidstudio-examples  # Android Studio's projects use the shared library built by this repo
├── tools                   # Tools and utilities
├── third_party             # Like OpenCV and [miniglog](https://github.com/tzutalin/miniglog)
├── CMakeLists.txt          # Use CMake to build instead of using Android.mk
├── LICENSE
└── README.md
```

### Do you want to contribute
 * If you have any improvement or you've found any bug, send a pull request with the code.
 * Give me a star on this repository

### Future tasks
* Add more examples to [dlib-android-app](https://github.com/tzutalin/dlib-android-app)

