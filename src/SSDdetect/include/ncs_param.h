#ifndef NCS_PARAM_H
#define NCS_PARAM_H

#include <mvnc.h>
#include <mutex>

// somewhat arbitrary buffer size for the device name
#define NAME_SIZE 100

#define STB_IMAGE_RESIZE_STATIC
#include "stb_image.h"
#ifndef STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"
#endif

//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb_image_write.h"

typedef unsigned short half;

extern "C" {
#include "fp16.h"
//#include "image.h"
}
#include "stb_image_write.h"

// OpenCv
#include "image_interface.h"
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/objdetect/objdetect.hpp>
//#include <cv_bridge/cv_bridge.h>

//ncs flag for ssd
extern  bool g_graph_Success;
extern  bool g_image_Success;
extern  mvncStatus retCode;
extern  void *deviceHandle;
extern  char devName[NAME_SIZE];
extern  unsigned int graphFileLen;
extern  void* graphHandle;
extern  void* graphFileBuf;

//ncs flag for facenet
extern  bool g_graph_Success_face;
extern  bool g_image_Success_face;
extern  mvncStatus retCode_face;
extern  void *deviceHandle_face;
extern  char devName_face[NAME_SIZE];
extern  unsigned int graphFileLen_face;
extern  void* graphHandle_face;
extern  void* graphFileBuf_face;


//mutex lock
extern std::mutex mutexLaserSet;
extern std::mutex mutexSelectId;
extern std::mutex mutexCameraRects;

//extern "C" void ipl_into_image(IplImage* src, image im);
//extern "C" image ipl_to_image(IplImage* src);

extern void ipl_into_image(IplImage* src, image* im);
extern image* ipl_to_image(IplImage* src);

extern half *LoadImage(unsigned char *img, int reqsize, int width, int height);
extern half *LoadImage_face(unsigned char *img, int reqsize, int width, int height);
extern void *LoadFile(const char *path, unsigned int *length);
extern unsigned char* image_to_stb(image* in);
extern unsigned char* cvMat_to_charImg(cv::Mat pic);
//extern unsigned IplImage* cvMat_to_charImg(cv::Mat pic);
//void save_image_png(image im, char *name);



#endif
