#include "image_interface.h"


image* ipl_to_image(IplImage* src)
{
  int h = src->height;
  int w = src->width;
  int c = src->nChannels;

  image* out = new image(w,h,c);

  ipl_into_image(src, out);
  return out;
}


void ipl_into_image(IplImage* src, image* im)
{
//  unsigned char *data = (unsigned char *)src->imageData;
  int h = src->height;
  int w = src->width;
  int c = src->nChannels;
  int step = src->widthStep;
  int i, j, k;
 // std::cout << "h: " << h << " w: " << w << " c: " << c << std::endl;

  for(i = 0; i < h; ++i){
    for(k= 0; k < c; ++k){
      for(j = 0; j < w; ++j){
       im->data[k*w*h + i*w + j] = ((unsigned char *)src->imageData)[i*step + j*c + k]/255.;
      }
    }
  }
}

//void rgbgr_image(image* im)
//{
//  int i;
//  for(i = 0; i < im->w*im->h; ++i){
//    float swap = im->data[i];
//    im->data[i] = im->data[i+im->w*im->h*2];
//    im->data[i+im->w*im->h*2] = swap;
//  }
//}