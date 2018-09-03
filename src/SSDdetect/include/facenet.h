#ifndef FACENET_H_ZIWEI
#define FACENET_H_ZIWEI

#pragma once
#include <math.h>

#include "ncs_param.h"

extern "C" {
#include "fp16.h"
}

// 16 bits.  will use this to store half precision floats since C++ has no
// built in support for it.
half  *whitenImg(float *imgfp32, int reqsize, half *imgfp16);
float *compute(cv::Mat imageMat, void* graphHandle);

#endif