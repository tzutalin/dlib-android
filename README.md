# dlib-android

### Purpose
* Port [dlib](http://dlib.net/) to Andriod platform

* You can build it to dynamic or static library for Android. You can also build dlib's sample to Android executable file.

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

* Prepare a Android device for test

### Grab the source

    $ cd ~
    $ git clone https://github.com/tzutalin/dlib-android.git


### Build and Run
You can change the compiler architecture in dlib-android/jni/Application.mk

Build examples (example/svm.ex.cpp, examples/svr_ex.cpp, examples/max_cost_assignment_ex.cpp). Compiler architecture is armeabi-v7a

    $ cd ~/dlib-android
    $ sh buildandRun.sh

Output:

	[armeabi-v7a] Compile++ thumb: max_cost_assignment_ex <= max_cost_assignment_ex.cpp
	[armeabi-v7a] Compile++ thumb: dlib <= save_jpeg.cpp
	[armeabi-v7a] Compile++ thumb: dlib <= save_png.cpp
	[armeabi-v7a] StaticLibrary  : libdlib.a
	[armeabi-v7a] Executable     : max_cost_assignment_ex
	[armeabi-v7a] Install        : max_cost_assignment_ex => libs/armeabi-v7a/max_cost_assignment_ex
	[armeabi-v7a] Compile++ thumb: svm_ex <= svm_ex.cpp
	[armeabi-v7a] Executable     : svm_ex
	[armeabi-v7a] Install        : svm_ex => libs/armeabi-v7a/svm_ex
	[armeabi-v7a] Compile++ thumb: svr_ex <= svr_ex.cpp
	[armeabi-v7a] Executable     : svr_ex
	[armeabi-v7a] Install        : svr_ex => libs/armeabi-v7a/svr_ex
    ....
	.... svm example
	doing cross validation
	gamma: 1e-05    nu: 1e-05     cross validation accuracy: 0.926984 0.347283 
	gamma: 1e-05    nu: 5e-05     cross validation accuracy: 0.926984 0.347283 
	gamma: 1e-05    nu: 0.00025     cross validation accuracy: 0.926984 0.347283
    ....
	....  max_cost_assignment example
	number of support vectors in our learned_function is 269
	This is a +1 class example, the classifier output is 2.24922
	This is a +1 class example, the classifier output is 0.10343
    ....
	.... svr example
	0.239389   0.23905
	0.998334   0.997331
	-0.189201   -0.187636
	-0.191785   -0.218924
	MSE and R-Squared: 2.73043e-05    0.999842
You can build only (Optional)

	$ ndk-build 

###Notes:
* If you would like to integrate NDK into Eclipse, please refer to [android-ndk-eclipse-setup](http://tzutalin.blogspot.tw/2015/08/android-ndk-eclipse-setup.html) or [Android developer](https://developer.android.com/ndk/guides/ndk-build.html)

* TODO: Some of examples are not able to build succesfully. 
* Add dlib as a submodule
