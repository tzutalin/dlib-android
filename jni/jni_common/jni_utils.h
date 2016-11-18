#ifndef JNI_UTILS_H
#define JNI_UTILS_H
/*
 *     Author : Darren
 * Created on : 06/20 2016
 *
 * Copyright (c) 2016 Darren. All rights reserved.
 */
#include <jni.h>
#include <string>

namespace jniutils {

char* convertJStrToCStr(JNIEnv* env, jstring lString);

std::string convertJStrToString(JNIEnv* env, jstring lString);

JNIEnv* vm2env(JavaVM* vm);

}  // end jniutils

#endif /* JNI_UTILS_H */
