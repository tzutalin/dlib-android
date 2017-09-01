/*
 * rgb2yuv.h using google-style
 *
 *  Created on: May 24, 2016
 *      Author: Tzutalin
 *
 *  Copyright (c) 2016 Tzutalin. All rights reserved.
 */

#pragma once
#include <jni_common/types.h>
namespace jnicommon {

#ifdef __cplusplus
extern "C" {
#endif

void ConvertARGB8888ToYUV420SP(const uint32* const input, uint8* const output,
                               int width, int height);

void ConvertRGB565ToYUV420SP(const uint16* const input, uint8* const output,
                             const int width, const int height);
#ifdef __cplusplus
}  // end jnicommon
}
#endif
