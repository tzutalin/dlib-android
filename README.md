# dlib-android

### Purpose
* Port [dlib](http://dlib.net/) to Andriod platform

* You can build it to dynamic or static library for Android. You can also build dlib's sample to Android executable file.

* You can refer to [dlib-android-app](https://github.com/tzutalin/dlib-android-app) which demonstartes dlib-android features

### Grab the source

    $ cd ~
    $ git clone --recursive https://github.com/tzutalin/dlib-android.git

### Prerequisites
* Download Android-NDK from [Android website](https://developer.android.com/ndk/downloads/index.html).

	 After downloading, go to the directory to which you downloaded the package to extract it

	 `$ chmod a+x android-ndk-r10e-linux-x86_64.bin`

	 `$ ./android-ndk-r10e-linux-x86_64.bin`

	 Export ANDROID_NDK_HOME in ~/.bashrc
     `$ vim ~/.bashrc`

	`export ANDROID_NDK_HOME=[/to/your/path]/android-ndk-r10e`

    `export PATH=$PATH:$ANDROID_NDK_HOME`

* Install Android Debug Bride (ADB). You can download it via [Android SDK Manager](https://developer.android.com/sdk/installing/index.html) or $ sudo apt-get install android-tools-adb

* [Optional] You can download a specific Android-OpenCV from http://opencv.org/downloads.html.  Download the version and copy the folders in native SDK to dlib-android/opencv

* Prepare a Android device for test


### Build and Run
You can change the compiler architecture in dlib-android/jni/Application.mk

Build executable files and shared library, and push the executable files to Android devices. 

    $ cd [$dlib-android]
    
    Run a test daemon sample on your arm devices
    $ python build_push.py --test true
    
    Copy the shared lib to your Android project
    $ python build_push.py --android_project dlib-android-appliciton/dlib/src/main/jniLibs/

You can build only (Optional)

	$ ndk-build -j 2

### Write JNI and build shared library for Android Java application
There are examples in [jnilib_ex](https://github.com/tzutalin/dlib-android/tree/master/jni/jnilib_ex)

There is a sample app to demonstrate the result. [dlib-android-app](https://github.com/tzutalin/dlib-android-app) 

### Do you want to contribute
 If you have any improvement or you've found any bug, send a pull request with the code 


###Notes:
* If you would like to integrate NDK into Eclipse, please refer to [android-ndk-eclipse-setup](http://tzutalin.blogspot.tw/2015/08/android-ndk-eclipse-setup.html) or [Android developer](https://developer.android.com/ndk/guides/ndk-build.html)


### Future task
* Add more examples to [dlib-anroid-app](https://github.com/tzutalin/dlib-android-app)

* Intgrate to Android - studio using gradle or Bazel build
