#ifndef IMAGE_INTERFACE_H
#define IMAGE_INTERFACE_H


#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <string.h>
#include <math.h>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <cv_bridge/cv_bridge.h>

#include <boost/smart_ptr.hpp>
#include <boost/make_shared.hpp>

class image{
public:
    int w;
    int h;
    int c;
    float *data;
    image(){
        data = nullptr;
    };
    image(int _w, int _h, int _c){
        w = _w;
        h = _h;
        c = _c;
        data = new float[h*w*c]();
    };
    ~image(){
        delete[] data;
    };
};


image* ipl_to_image(IplImage* src);
void ipl_into_image(IplImage* src, image* im);
//image make_image(int w, int h, int c);
//image make_empty_image(int w, int h, int c);
void rgbgr_image(image im);



#endif
