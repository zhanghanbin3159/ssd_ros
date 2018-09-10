#include <image_interface.h>
#include "ncs_param.h"

//ncs flag for yolo
bool g_graph_Success;
bool g_image_Success;
mvncStatus retCode;
void *deviceHandle;
char devName[NAME_SIZE];
unsigned int graphFileLen;
void* graphHandle;
void* graphFileBuf;

//ncs flag for facenet
bool g_graph_Success_face;
bool g_image_Success_face;
mvncStatus retCode_face;
void *deviceHandle_face;
char devName_face[NAME_SIZE];
unsigned int graphFileLen_face;
void* graphHandle_face;
void* graphFileBuf_face;

//mutex lock
std::mutex mutexLaserSet;
std::mutex mutexSelectId;
std::mutex mutexCameraRects;

half *LoadImage(unsigned char *img, int reqsize, int width, int height)
{
    int i;
    unsigned char *imgresized;
    float *imgfp32;
    half *imgfp16;

    if(!img)
    {
        printf("The picture  could not be loaded\n");
        return 0;
    }
    imgresized = (unsigned char*) malloc(3*reqsize*reqsize);
    if(!imgresized)
    {
        free(img);
        perror("malloc");
        return 0;
    }
    //std::cout << "img: " << img << std::endl;
    stbir_resize_uint8(img, width, height, 0, imgresized, reqsize, reqsize, 0, 3);
    free(img);
    imgfp32 = (float*) malloc(sizeof(*imgfp32) * reqsize * reqsize * 3);
    if(!imgfp32)
    {
        free(imgresized);
        perror("malloc");
        return 0;
    }
    for(i = 0; i < reqsize * reqsize * 3; i++)
        imgfp32[i] = imgresized[i];
    free(imgresized);
    imgfp16 = (half*) malloc(sizeof(*imgfp16) * reqsize * reqsize * 3);
    if(!imgfp16)
    {
        free(imgfp32);
        perror("malloc");
        return 0;
    }
    //adjust values to range between -1.0 and + 1.0
    //change color channel
    for(i = 0; i < reqsize*reqsize; i++)
    {
        float blue, green, red;
        blue = imgfp32[3*i+2];
        green = imgfp32[3*i+1];
        red = imgfp32[3*i+0];
        imgfp32[3*i+0] = (blue-127.5)*0.007843;
        imgfp32[3*i+1] = (green-127.5)*0.007843;
        imgfp32[3*i+2] = (red-127.5)*0.007843;
        // uncomment to see what values are getting passed to mvncLoadTensor() before conversion to half float
        //printf("Blue: %f, Grean: %f,  Red: %f \n", imgfp32[3*i+0], imgfp32[3*i+1], imgfp32[3*i+2]);
    }
    floattofp16((unsigned char *)imgfp16, imgfp32, 3*reqsize*reqsize);
    free(imgfp32);
    return imgfp16;
}


half *LoadImage_face(unsigned char *img, int reqsize, int width, int height)
{
    int i;
    unsigned char *imgresized;
    float *imgfp32;
    half *imgfp16;

    if(!img)
    {
        printf("The picture  could not be loaded\n");
        return 0;
    }
    imgresized = (unsigned char*) malloc(3*reqsize*reqsize);
    if(!imgresized)
    {
        free(img);
        perror("malloc");
        return 0;
    }
    std::cout << "face img: " << img << std::endl;
    stbir_resize_uint8(img, width, height, 0, imgresized, reqsize, reqsize, 0, 3);
    free(img);
    imgfp32 = (float*) malloc(sizeof(*imgfp32) * reqsize * reqsize * 3);
    if(!imgfp32)
    {
        free(imgresized);
        perror("malloc");
        return 0;
    }
    for(i = 0; i < reqsize * reqsize * 3; i++)
        imgfp32[i] = imgresized[i];
    free(imgresized);
    imgfp16 = (half*) malloc(sizeof(*imgfp16) * reqsize * reqsize * 3);
    if(!imgfp16)
    {
        free(imgfp32);
        perror("malloc");
        return 0;
    }
    for(i = 0; i < reqsize*reqsize; i++)
    {
        float blue, green, red;
        blue = imgfp32[3*i+2];
        green = imgfp32[3*i+1];
        red = imgfp32[3*i+0];

        imgfp32[3*i+0] = (blue)/255; //74
        imgfp32[3*i+1] = (green)/255; //23
        imgfp32[3*i+2] = (red)/255; //47
        // uncomment to see what values are getting passed to mvncLoadTensor() before conversion to half float
        //printf("Blue: %f, Grean: %f,  Red: %f \n", imgfp32[3*i+0], imgfp32[3*i+1], imgfp32[3*i+2]);
    }

    //对人脸照片做处理
    float sum = 0.0;
    for(int i = 0; i < reqsize * reqsize * 3; i++){
        sum += imgfp32[i];
    }
    // printf("sum: %f\n",sum);

    float source_mean = sum/(reqsize * reqsize * 3);
    // printf("source_mean: %f\n",source_mean);

    float temp1 = 0.0;
    for(int i = 0; i < reqsize * reqsize * 3; i++){
        temp1 += pow(imgfp32[i]-source_mean,2);
    }

    float source_standard_deviation = sqrt(temp1/(reqsize * reqsize * 3));
    // printf("source_standard_deviation: %f\n", source_standard_deviation);

    float std_adjusted = source_standard_deviation > float (1.0/sqrt(reqsize * reqsize * 3)) ? source_standard_deviation : float (1.0/sqrt(reqsize * reqsize * 3));
    // printf("std_adjusted: %f\n",std_adjusted);

    float *whitened_image = (float *)malloc(sizeof(*whitened_image) * reqsize * reqsize * 3);
    for(int i =0; i < reqsize * reqsize * 3; i++){
        whitened_image[i] = (imgfp32[i] - source_mean)*(1.0/std_adjusted);
//        printf("whitened_image %d : %f \n" , i, whitened_image[i]);
    }

    floattofp16((unsigned char *)imgfp16, whitened_image, 3*reqsize*reqsize);
    free(imgfp32);
    free(whitened_image);
    return imgfp16;
}


void *LoadFile(const char *path, unsigned int *length)
{
    FILE *fp;
    char *buf;

    fp = fopen(path, "rb");
    if(fp == NULL)
        return 0;
    fseek(fp, 0, SEEK_END);
    *length = ftell(fp);
    rewind(fp);
    if(!(buf = (char*) malloc(*length)))
    {
        fclose(fp);
        return 0;
    }
    if(fread(buf, 1, *length, fp) != *length)
    {
        fclose(fp);
        free(buf);
        return 0;
    }
    fclose(fp);
    return buf;
}

unsigned char* image_to_stb(image* in)
{
    int i,j,k;
    int w = in->w;
    int h = in->h;
    int c =3;
    unsigned char *img = (unsigned char*) malloc(c*w*h);
    for(k = 0; k < c; ++k){
        for(j=0; j <h; ++j){
            for(i=0; i<w; ++i){
                int src_index = i + w*j + w*h*k;
                int dst_index = k + c*i + c*w*j;
                img[dst_index] = (unsigned char)(255*in->data[src_index]);
            }
        }
    }
    delete in;
    // std::cout << "xxxxx" << std::endl;
    return img;
}


unsigned char* cvMat_to_charImg(cv::Mat pic)
{
    IplImage copy = pic;
    IplImage *pic_Ipl = &copy;



    image* buff_;
    buff_ = ipl_to_image(pic_Ipl);
    unsigned char* pic_final = image_to_stb(buff_);

    //cvReleaseImage(&pic_Ipl);

    return pic_final;
}


//void save_image_png(image im, char *name)
//{
//    char buff[256];
//    //sprintf(buff, "%s (%d)", name, windows);
//    sprintf(buff, "%s.png", name);
//    unsigned char *data = (unsigned char*)calloc(im.w*im.h*im.c, sizeof(char));
//    int i,k;
//    for(k = 0; k < im.c; ++k){
//        for(i = 0; i < im.w*im.h; ++i){
//            data[i*im.c+k] = (unsigned char) (255*im.data[i + k*im.w*im.h]);
//        }
//    }
//    int success = stbi_write_png(buff, im.w, im.h, im.c, data, im.w*im.c);
//    free(data);
//    if(!success) fprintf(stderr, "Failed to write image %s\n", buff);
//}