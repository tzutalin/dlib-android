# miniglog

A logging C++ library based on the miniglog library found in [ceres-solver](https://android.googlesource.com/platform/external/ceres-solver/+/refs/heads/lollipop-cts-release/internal/ceres/miniglog/glog)

Let people include and build c++ logging quickly

### Benefits
* Very small (less than 1000 LOC)
* Easy to use
* No 3rd-party dependencies
* Cross-platform
* Same interface as glog

### Grab the source

    $ cd ~
    $ git clone https://github.com/tzutalin/miniglog.git

### Build
* Ubuntu/Mac/Windows

   `mkdir build; cd build`

   `cmake -D CMAKE_INSTALL_PREFIX=. ..; make -j8; make install`

   Build and run test sample

   `cd ../example/test/; cmake .;make`

   `./test`

* Android build

   `cd example`

   `ndk-build`

### Usage

LOG Level: FATAL ERROR WARNING INFO

```
	#include <glog/logging.h>
    // Log level is INFO
    LOG(INFO) << "Dump log test";
    // If the marco start with 'D', stands for Debug only
    DLOG(INFO) << "Dump log test";

    // You can aslo use other check marco like as below
    DCHECK_EQ(val1, val2)
    DCHECK_NE(val1, val2)
    DCHECK_LE(val1, val2)
    DCHECK_LT(val1, val2)
    DCHECK_GE(val1, val2)
    DCHECK_GT(val1, val2)

	// You may append to the error message like so:
	CHECK_NE(1, 2) << ": The world must be ending!";

	// Check if it is euqual
	CHECK_EQ(string("abc")[1], 'b');

	// Better yet, use the CHECK_NOTNULL macro
    CHECK_NOTNULL(some_ptr);
	some_ptr->DoSomething();
```

You can import it quickly to your Android.mk
```
    #MINIGLOG_LIB_TYPE:=SHARED
    MINIGLOG_LIB_TYPE:=STATIC
 
    ifeq ($(MINIGLOG_LIB_TYPE),SHARED)
        LOCAL_SHARED_LIBRARIES += miniglog
    else
        LOCAL_STATIC_LIBRARIES += miniglog
    endif
 
    include [Miniglog folder]/Android.mk
```

You can refer to the sample code

Android example and Android.mk: You can refer to

[./example/jni](./example/jni)


[More info about the usage of glog](http://rpg.ifi.uzh.ch/docs/glog.html)
