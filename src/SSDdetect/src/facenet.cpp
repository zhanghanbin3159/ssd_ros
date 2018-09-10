#include "facenet.h"
const int face_networkDim = 160;

half *whitenImg(float *imgfp32, int reqsize, half *imgfp16)
{
    //对人脸照片做处理
    float sum = 0.0;
    for(int i = 0; i < reqsize * reqsize * 3; i++){
        sum += imgfp32[i];
    }
    //printf("sum: %f\n",sum);

    float source_mean = sum/(reqsize * reqsize * 3);
    //printf("source_mean: %f\n",source_mean);

    float temp1 = 0.0;
    for(int i = 0; i < reqsize * reqsize * 3; i++){
        temp1 += pow(imgfp32[i]-source_mean,2);
    }

    float source_standard_deviation = sqrt(temp1/(reqsize * reqsize * 3));
    //printf("source_standard_deviation: %f\n", source_standard_deviation);

    float std_adjusted = source_standard_deviation > float (1.0/sqrt(reqsize * reqsize * 3)) ? source_standard_deviation : float (1.0/sqrt(reqsize * reqsize * 3));
    //printf("std_adjusted: %f\n",std_adjusted);

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

float *compute(cv::Mat imageMat, void* graphHandle_face)
{
    mvncStatus retCode_face;
    float *resultData32;
    //std::cout << "compute" << std::endl;

    // successfully allocated graph.  Now graphHandle is ready to go.
    // use graphHandle for other API calls and call mvncDeallocateGraph
    // when done with it.
   // printf("Successfully allocated graph for %s\n", GRAPH_FILE_NAME);

    // LoadImage will read image from disk, convert channels to floats
    // subtract network mean for each value in each channel.  Then, convert
    // floats to half precision floats and return pointer to the buffer
    // of half precision floats (Fp16s)
    // 加载照片
    unsigned char* buff_ = cvMat_to_charImg(imageMat);
    half *imageBufFp16 = LoadImage_face(buff_, face_networkDim, imageMat.cols, imageMat.rows);

    // calculate the length of the buffer that contains the half precision floats.
    // 3 channels * width * height * sizeof a 16bit float
    unsigned int lenBufFp16 = 3 * face_networkDim * face_networkDim * sizeof(*imageBufFp16);

    // start the inference with mvncLoadTensor()
    // inference 出结果，保存在lenBufFp16中
    retCode_face = mvncLoadTensor(graphHandle_face, imageBufFp16, lenBufFp16, NULL);

    if (retCode_face != MVNC_OK) {   // error loading tensor
        printf("Could not load face tensor\n");
        printf("Error from mvncLoadTensor is: %d\n", retCode_face);
    } else {   // the inference has been started, now call mvncGetResult() for the
        // inference result
       // printf("Successfully loaded the tensor for face image \n");

        void *resultData16;
        void *userParam;
        unsigned int lenResultData;
        retCode_face = mvncGetResult(graphHandle_face, &resultData16, &lenResultData, &userParam);
        if (retCode_face == MVNC_OK) {
            // Successfully got the result.  The inference result is in the buffer pointed to by resultData
           // printf("Successfully got the inference result for image \n");
           // printf("resultData is %d bytes which is %d 16-bit floats.\n", lenResultData,
//                   lenResultData / (int) sizeof(half));

            // convert half precision floats to full floats
            int numResults = lenResultData / sizeof(half);

            resultData32 = (float *) malloc(numResults * sizeof(*resultData32));
            fp16tofloat(resultData32, (unsigned char *) resultData16, numResults);
        }
    }
    return resultData32;
}

