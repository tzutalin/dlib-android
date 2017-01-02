/*
 * Copyright (c) 2016 Tzutalin
 * Create by TzuTaLin <tzu.ta.lin@gmail.com>
 */

#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

/*
 *     Author : TzuTaLin
 * Created on : 07/22 2016
 *
 * Copyright (c) 2016 Tzutalin. All rights reserved.
 */
namespace jniutils {

bool fileExists(const char* name);

bool dirExists(const char* name);

bool fileExists(const std::string& name);

bool dirExists(const std::string& name);

}  // end jniutils
   /* FILEUTILS_H */
