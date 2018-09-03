#include "image_interface.h"


image ipl_to_image(IplImage* src)
{
  int h = src->height;
  int w = src->width;
  int c = src->nChannels;
  image out = make_image(w, h, c);
  ipl_into_image(src, out);
  return out;
}


void ipl_into_image(IplImage* src, image im)
{
  unsigned char *data = (unsigned char *)src->imageData;
  int h = src->height;
  int w = src->width;
  int c = src->nChannels;
  int step = src->widthStep;
  int i, j, k;
 // std::cout << "h: " << h << " w: " << w << " c: " << c << std::endl;
  for(i = 0; i < h; ++i){
    for(k= 0; k < c; ++k){
      for(j = 0; j < w; ++j){

        //std::cout << "index: "<< k*w*h + i*w + j << " data: " << data[i*step + j*c + k]/255. << " i: " << i << " k："<< k << " j:" << j << " w: "<< w <<" h: "<<h <<" c: "<< c << std::endl;
        im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
      }
    }
  }
}

image make_image(int w, int h, int c)
{
  image out = make_empty_image(w,h,c);
//  out.data = calloc(h*w*c, sizeof(float));
  return out;
}

image make_empty_image(int w, int h, int c)
{
  image out;
//  out.data = 0;
  out.data = new float[h*w*c]();//calloc(h*w*c, sizeof(float));
  out.h = h;
  out.w = w;
  out.c = c;
  return out;
}

void rgbgr_image(image im)
{
  int i;
  for(i = 0; i < im.w*im.h; ++i){
    float swap = im.data[i];
    im.data[i] = im.data[i+im.w*im.h*2];
    im.data[i+im.w*im.h*2] = swap;
  }
}