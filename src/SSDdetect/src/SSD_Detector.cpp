// ssd object detector
#include "SSD_Detector.hpp"

// Check for xServer
#include <X11/Xlib.h>

//#ifdef SSD_FILE_PATH
//std::string ssdFilePath_ = SSD_FILE_PATH;
//#else
//#error Path of darknet repository is not defined in CMakeLists.txt.
//#endif

//#define STB_IMAGE_RESIZE_STATIC
//#include "stb_image.h"
//#define STB_IMAGE_RESIZE_IMPLEMENTATION
//#include "stb_image_resize.h"
std::map<int, std::string> LABELS = {{0,"background"}, {1, "person"}, {2, "face"}};
// YOLO image dimensions, network mean values for each channel in BGR order.
const int networkDim = 300;

inline float getOverlap(const cv::Rect &b1, const cv::Rect &b2) //b1是激光的框，b2是摄像头识别的人
{
#define min___(a, b) (a > b ? b : a)
#define max___(a, b) (a < b ? b : a)
    int ws1 = min___(b1.x + b1.width, b2.x + b2.width) - max___(b1.x, b2.x);
    int hs1 = min___(b1.y + b1.height, b2.y + b2.height) - max___(b1.y, b2.y);
    float o = max___(0, ws1) * max___(0, hs1);
    o = o / (b2.width * b2.height);
    //o = o / (b1.width * b1.height + b2.width * b2.height - o);
    return o;
}


//num是laser 框的数量
void draw_laser_box(image im, int num, std::vector<cv::Rect> laser_rects)
{
    //xmin = (rosBoxes_[k][j].x - rosBoxes_[k][j].w / 2) * frameWidth_
    int i;
    for(i = 0; i < num; ++i){

        int width = im.h * .006;

        float red = 1;
        float green = 1;
        float blue = 1;
        float rgb[3];

        rgb[0] = red;
        rgb[1] = green;
        rgb[2] = blue;
        //box b = laser_rects[i];

//        int left  = (b.x-b.w/2.)*im.w;
//        int right = (b.x+b.w/2.)*im.w;
//        int top   = (b.y-b.h/2.)*im.h;
//        int bot   = (b.y+b.h/2.)*im.h;

        int left  = laser_rects[i].x;
        int right = laser_rects[i].x + laser_rects[i].width;
        int top   = laser_rects[i].y;
        int bot   = laser_rects[i].y + laser_rects[i].height;

        if(left < 0) left = 0;
        if(right > im.w-1) right = im.w-1;
        if(top < 0) top = 0;
        if(bot > im.h-1) bot = im.h-1;
        //std::cout << "before laser draw" << std::endl;
       // draw_box_width(im, left, top, right, bot, width, red, green, blue);
    }
}

float* array_transform(float* resultData32, int w, int h, int num)
{
    float* prediction;
	prediction = (float*)malloc(num * sizeof(*prediction));
    int c = num/(w*h);

    int i,j,k;
    for(k = 0; k < c; ++k){
        for(j=0; j <h; ++j){
            for(i=0; i<w; ++i){
                int dst_index= i + w*j + w*h*k;
                int src_index = k + c*i + c*w*j;
                prediction[dst_index] = resultData32[src_index];
            }
        }
    }
    resultData32 = prediction;
    free(prediction);
    return resultData32;
}


////将对应buffer中的数据做缓存
//void img2Ipl(image p, const char *name, IplImage *disp) {
//    int x, y, k;
//    if (p.c == 3) rgbgr_image(p);
//    //normalize_image(copy);
//
//    char buff[256];
//    //sprintf(buff, "%s (%d)", name, windows);
//    sprintf(buff, "%s", name);
//
//    int step = disp->widthStep;
//    for (y = 0; y < p.h; ++y) {
//        for (x = 0; x < p.w; ++x) {
//            for (k = 0; k < p.c; ++k) {
//                disp->imageData[y * step + x * p.c + k] = (unsigned char) (get_pixel(p, x, y, k) * 255);
//            }
//        }
//    }
//}

namespace ssd_ros {

    SSD_Detector::SSD_Detector(ros::NodeHandle nh)
            : nodeHandle_(nh),
              imageTransport_(nodeHandle_),
              numClasses_(0),
              classLabels_(0),
              rosBoxes_(0),
              rosBoxCounter_(0) {
        ROS_INFO("[SSD_Detector] Node started.");

        // Read parameters from config file.
        if (!readParameters()) {
            ros::requestShutdown();
        }
        g_graph_Success = false;
        g_image_Success = false;
        init_ncs();
        init();
    }

    SSD_Detector::~SSD_Detector() {
        {
            boost::unique_lock <boost::shared_mutex> lockNodeStatus(mutexNodeStatus_);
            isNodeRunning_ = false;
        }

        //clear and close ncs
        ROS_INFO("Delete movidius SSD graph");
        retCode = mvncDeallocateGraph(graphHandle);
        graphHandle = NULL;

        free(graphFileBuf);
        retCode = mvncCloseDevice(deviceHandle);
        deviceHandle = NULL;
        free(deviceHandle);

        ssdThread_.join();
    }

    bool SSD_Detector::readParameters() {
        // Load common parameters.
        //std::cout << "read paramerters" << std::endl;
        nodeHandle_.param("image_view/enable_opencv", viewImage_, false);
        nodeHandle_.param("image_view/wait_key_delay", waitKeyDelay_, 3);
        nodeHandle_.param("image_view/enable_console_output", enableConsoleOutput_, false);


        // Set vector sizes.
        nodeHandle_.param("ssd_model/detection_classes/names", classLabels_,
                          std::vector<std::string>(0));
        numClasses_ = classLabels_.size();
        //rosBoxes_ = std::vector < std::vector < RosBox_ >> (numClasses_);
        rosBoxCounter_ = std::vector<int>(numClasses_);
        //std::cout<< "roscount: " << rosBoxes_.size() << std::endl;

        return true;
    }


    // ncsdk1
    void SSD_Detector::init_ncs() {
        retCode = mvncGetDeviceName(0, devName, NAME_SIZE);
        if (retCode != MVNC_OK) {   // failed to get device name, maybe none plugged in.
            printf("No NCS SSD devices found\n");
            exit(-1);
        }

        // Try to open the NCS device via the device name
        retCode = mvncOpenDevice(devName, &deviceHandle);
        if (retCode != MVNC_OK) {   // failed to open the device.
            printf("Could not open NCS device\n");
            exit(-1);
        }

        // deviceHandle is ready to use now.
        // Pass it to other NC API calls as needed and close it when finished.
        printf("Successfully opened NCS device!\n");

        // import graph to ncs
        std::string graphPath;
        std::string graphModel;
        // Path to graph file.
        nodeHandle_.param("ssd_model/graph_file/name", graphModel, std::string("mobilenet_ssd_graph"));
        nodeHandle_.param("graph_path", graphPath, std::string("/default"));
        graphPath += "/" + graphModel;
        GRAPH_FILE_NAME = new char[graphPath.length() + 1];
        strcpy(GRAPH_FILE_NAME, graphPath.c_str());

        // Now read in a graph file
        graphFileBuf = LoadFile(GRAPH_FILE_NAME, &graphFileLen);

        // allocate the graph
        retCode = mvncAllocateGraph(deviceHandle, &graphHandle, graphFileBuf, graphFileLen);
        if (retCode != MVNC_OK) {   // error allocating graph
            printf("Could not allocate graph for file: %s\n", GRAPH_FILE_NAME);
            printf("Error from mvncAllocateGraph is: %d\n", retCode);
        } else {
            printf("Successfully allocate graph for file: %s\n", GRAPH_FILE_NAME);
            g_graph_Success = true;
        }
    }

    void SSD_Detector::init() {
        ROS_INFO("[SSD_Detector] init().");

        // Initialize deep network of ssd_mobilenet.
        std::string configModel;

        // Threshold of object detection.
        float thresh;
        nodeHandle_.param("ssd_model/threshold/value", thresh, (float) 0.3);


        // Get classes.
        detectionNames = (char **) realloc((void *) detectionNames, (numClasses_ + 1) * sizeof(char *));
        for (int i = 0; i < numClasses_; i++) {
            detectionNames[i] = new char[classLabels_[i].length() + 1];
            strcpy(detectionNames[i], classLabels_[i].c_str());
        }

        // Load network.
        ssd_threshold = thresh;

        ssdThread_ = std::thread(&SSD_Detector::ssd, this);

        //load the parameters
        demoPrefix_ = 0;
        demoDelay_ = 0;
        demoFrame_ = 1;
        //demoNames_ = names;
//        demoClasses_ = classes;
        demoThresh_ = 0.5;
        demoHier_ = 0.5;
        fullScreen_ = 0;
        // Initialize publisher and subscriber.
        std::string cameraTopicName;
        int cameraQueueSize;
        std::string objectDetectorTopicName;
        int objectDetectorQueueSize;
        bool objectDetectorLatch;
        std::string boundingBoxesTopicName;
        int boundingBoxesQueueSize;
        bool boundingBoxesLatch;
        std::string detectionImageTopicName;
        int detectionImageQueueSize;
        bool detectionImageLatch;
        bool flip_flag;

        nodeHandle_.param("subscribers/camera_reading/topic", cameraTopicName,
                          std::string("/camera/image_raw"));
        nodeHandle_.param("subscribers/camera_reading/queue_size", cameraQueueSize, 1);
        nodeHandle_.param("publishers/object_detector/topic", objectDetectorTopicName,
                          std::string("found_object"));
        nodeHandle_.param("publishers/object_detector/queue_size", objectDetectorQueueSize, 1);
        nodeHandle_.param("publishers/object_detector/latch", objectDetectorLatch, false);
        nodeHandle_.param("publishers/bounding_boxes/topic", boundingBoxesTopicName,
                          std::string("bounding_boxes"));
        nodeHandle_.param("publishers/bounding_boxes/queue_size", boundingBoxesQueueSize, 1);
        nodeHandle_.param("publishers/bounding_boxes/latch", boundingBoxesLatch, false);
        nodeHandle_.param("publishers/detection_image/topic", detectionImageTopicName,
                          std::string("detection_image"));
        nodeHandle_.param("publishers/detection_image/queue_size", detectionImageQueueSize, 1);
        nodeHandle_.param("publishers/detection_image/latch", detectionImageLatch, true);
        nodeHandle_.param("camera/image_flip", flip_flag, false);

        imageSubscriber_ = imageTransport_.subscribe(cameraTopicName, cameraQueueSize,
                                                     &SSD_Detector::cameraCallback, this);
        objectPublisher_ = nodeHandle_.advertise<std_msgs::Int8>(objectDetectorTopicName,
                                                                 objectDetectorQueueSize,
                                                                 objectDetectorLatch);
        boundingBoxesPublisher_ = nodeHandle_.advertise<ssddetect::BoundingBoxes>(
                boundingBoxesTopicName, boundingBoxesQueueSize, boundingBoxesLatch);
        detectionImagePublisher_ = nodeHandle_.advertise<sensor_msgs::Image>(detectionImageTopicName,
                                                                             detectionImageQueueSize,
                                                                             detectionImageLatch);
        flipFlag = flip_flag;
        //std::cout << "flipFlag: " << flipFlag << std::endl;

    }

    void SSD_Detector::cameraCallback(const sensor_msgs::ImageConstPtr &msg) {
        ROS_DEBUG("[SSD_Detector] USB image received.");

        cv_bridge::CvImagePtr cam_image;

        try {
            cam_image = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        } catch (cv_bridge::Exception &e) {
            ROS_ERROR("cv_bridge exception: %s", e.what());
            return;
        }

        if (cam_image) {
            {
                //读写锁，这里boost::unique_lock<boost::shared_mutex> 是写锁
                // mutexImageCallback_是boost::shared_mutex型
                //对同一个对象mutexImageCallback_，线程可以同时有多个boost::shared_lock<boost::shared_mutex>，
                // 这些boost::shared_lock<boost::shared_mutex>会阻塞任意一个企图获得boost::unique_lock<boost::shared_mutex>的线程，
                // 直到所有的boost::shared_lock<boost::shared_mutex>对象都析构。
                // 如果boost::unique_lock<boost::shared_mutex>首先获得了mutexImageCallback_，
                // 那么它会阻塞任意一个企图在mutexImageCallback_上获得boost::shared_lock<boost::shared_mutex>或者writeLock的线程。

                //所以为了避免线程冲突，可以用这种读写锁的形式，这里所住mutexImageCallback_，进行图像读入操作，直到camImageCopy_赋值完毕
                boost::unique_lock <boost::shared_mutex> lockImageCallback(mutexImageCallback_);
               // std::cout << "camImageCopy get" << std::endl;

                //flip
                cv::Mat image0 = cam_image->image.clone();
                IplImage copy = image0;
                IplImage *frame = &copy;
                //std::cout << "flipFlag: " << flipFlag << std::endl;
                if(flipFlag)
                    cvFlip(frame, NULL, 0); //翻转
                camImageCopy_ = cv::cvarrToMat(frame, true);


                //std::cout << "Camera Callback: Has Image" << std::endl;
                //std::cout << "camImageCopy_: " << camImageCopy_ << std::endl;
//                cv::Mat image0 = cam_image->image.clone();
//                // IplImage* iplimg;
//                // *iplimg = IplImage(image0);
//                IplImage copy = image0;
//                IplImage *frame = &copy;
//                //cvFlip(frame, NULL, 0); //翻转
//                camImageCopy_ = cv::cvarrToMat(frame, true);
                //cvConvert(camImageCopy_,frame);
            }
            {
                boost::unique_lock <boost::shared_mutex> lockImageStatus(mutexImageStatus_);
                //这里对imageStatus进行赋值操作，为避免另一线程yolo()在此时读取imageStatus_，在赋值前先将其锁住
//                std::cout << "imageStatus_" << std::endl;
                imageStatus_ = true;
            }
            frameWidth_ = cam_image->image.size().width;
            frameHeight_ = cam_image->image.size().height;
        }
        return;
    }


    bool SSD_Detector::publishDetectionImage(const cv::Mat &detectionImage) {
        if (detectionImagePublisher_.getNumSubscribers() < 1) //要求发再发
            return false;
        cv_bridge::CvImage cvImage;
        cvImage.header.stamp = ros::Time::now();
        cvImage.header.frame_id = "detection_image";
        cvImage.encoding = sensor_msgs::image_encodings::BGR8;
        cvImage.image = detectionImage;
        detectionImagePublisher_.publish(*cvImage.toImageMsg());
        ROS_DEBUG("Detection image has been published.");
        return true;
    }

    double SSD_Detector::getWallTime() {
        struct timeval time;
        if (gettimeofday(&time, NULL)) {
            return 0;
        }
        return (double) time.tv_sec + (double) time.tv_usec * .000001;
    }


//  a.	First fp16 value holds the number of valid detections = num_valid.
//  b.	The next 6 values are unused.
//  c.	The next (7 * num_valid) values contain the valid detections data
//      Each group of 7 values will describe an object/box These 7 values in order.
//       The values are:
//         0: image_id (always 0)
//         1: class_id (this is an index into labels)
//         2: score (this is the probability for the class)
//         3: box left location within image as number between 0.0 and 1.0
//         4: box top location within image as number between 0.0 and 1.0
//         5: box right location within image as number between 0.0 and 1.0
//         6: box bottom location within image as number between 0.0 and 1.0
    // reslut 解析
    void SSD_Detector::SSD_result_infer(float *output, std::vector <Box> &result, cv::Mat &image) {
        //std::cout << "SSD_result_infer" << std::endl;
        int num_valid_boxes = output[0];
        //std::cout << "num_valid_boxes: " << num_valid_boxes << std::endl;
        // clip the boxes to the image size incase network returns boxes outside of the image
        for (int box_index = 0; box_index < num_valid_boxes; box_index++) {
            int base_index = 7 + box_index * 7;
            //todo_ziwei: 省略判断每一组数值是否有效(inf, nan, etc)， 之后需补上
            float x1 =
                    0 > (int) (output[base_index + 3] * image.rows) ? 0 : (int) (output[base_index + 3] * image.rows);
            float y1 =
                    0 > (int) (output[base_index + 4] * image.cols) ? 0 : (int) (output[base_index + 4] * image.cols);
            float x2 = image.rows < (int) (output[base_index + 5] * image.rows) ? image.rows : (int) (
                    output[base_index + 5] * image.rows);
            float y2 = image.cols < (int) (output[base_index + 6] * image.cols) ? image.cols : (int) (
                    output[base_index + 6] * image.cols);

//        std::cout << "box at index: " << box_index << " ClassID: " << LABELS[(int)output[base_index + 1]]<< " Confidence: " << output[base_index + 2]
//                  << " Top Left: " << x1 << "," << y1 << " Bottom Right:" << x2 << "," << y2 << std::endl;

            //用于图像显示box和label
            float output_pass[7];
            for (int i = 0; i < 7; i++) {
                output_pass[i] = output[base_index + i];
            }
            Box single_box;

            if (Overlay_on_image(image, output_pass, 7, single_box))
                result.push_back(single_box);
        }
    }

    //filter some boxes of which score lower than threshold
    bool SSD_Detector::Overlay_on_image(cv::Mat &image, float *object_info, int Length, Box &single_box) {
        // int min_score_percent = 60;
        int min_score_percent = int(ssd_threshold * 100);
        int source_image_width = image.cols;
        int source_image_height = image.rows;
        int base_index = 0;
        int class_id = object_info[base_index + 1];
        int percentage = int(object_info[base_index + 2] * 100);
        if (percentage < min_score_percent)
            return false;

//        std::cout << "source_image_width: " << source_image_width << " source_image_height: " << source_image_height
//                  << std::endl;
        int box_left = (int) (object_info[base_index + 3] * source_image_width);
        int box_top = (int) (object_info[base_index + 4] * source_image_height);
        int box_right = (int) (object_info[base_index + 5] * source_image_width);
        int box_bottom = (int) (object_info[base_index + 6] * source_image_height);
        int box_width = box_right - box_left;
        int box_height = box_bottom - box_top;
        cv::Rect box;
        box.x = box_left;
        box.y = box_top;
        box.width = box_width;
        box.height = box_height;

        int label_index = (int) object_info[base_index + 1];
        std::string label = LABELS[(int) object_info[base_index + 1]];
        int red_level = (int) (255.0 / LABELS.size()) * label_index;
        if(label_index==1)
        {
            cv::rectangle(image, box, cv::Scalar(red_level, 255, 255), 2);
            cv::putText(image, label,
                        cv::Point(box.x, box.y),
                        cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(red_level, 255, 255), 1, CV_AA);
        }
//        std::cout << "box at index: " << label_index << " ClassID: " << LABELS[(int) object_info[base_index + 1]]
//                  << " Confidence: " << object_info[base_index + 2]
//                  << " Top Left: " << box_top << "," << box_left << " Bottom Right:" << box_right << "," << box_bottom
//                  << std::endl;
        single_box.label = label_index;
        single_box.x = box_left;
        single_box.y = box_top;
        single_box.width = box_width;
        single_box.height = box_height;
        single_box.prob = object_info[base_index + 2];
        return true;
    }


    void *SSD_Detector::fetchInThread() {
        //printf("Fetch an image!\n");
        cv::Mat ROS_img = getCVImage();
//        buff_[buffIndex_].release();
//        buff_original[buffIndex_].release();

        ROS_img.copyTo(buff_[buffIndex_]);
        ROS_img.copyTo(buff_original[buffIndex_]);

//        buff_[buffIndex_] = ROS_img;
//        buff_original[buffIndex_] = ROS_img;
        buff_time[buffIndex_] = ros::Time::now();
        ROS_img.release();
        return 0;
    }

    // mobilenetssd infer 修改成ncsdk2
    void *SSD_Detector::detectInThread() {
       // printf("Detect an image!\n");
        running_ = 1;
        float nms = 0.3;
        cv::Mat in = buff_original[(buffIndex_ + 2) % 3]; //三张缓冲中的一张
        unsigned char *img = cvMat_to_charImg(in);

        use_time = buff_time[(buffIndex_ + 2) % 3];

        unsigned int graphFileLen;
        half *imageBufFp16 = LoadImage(img, networkDim, in.cols, in.rows); //***

        // calculate the length of the buffer that contains the half precision floats.
        // 3 channels * width * height * sizeof a 16bit float
        unsigned int lenBufFp16 = 3 * networkDim * networkDim * sizeof(*imageBufFp16);
        //std::cout << "networkDim: " << networkDim << " imageBufFp16: " << sizeof(*imageBufFp16) << " lenBufFp16: " << lenBufFp16 << std::endl;
        retCode = mvncLoadTensor(graphHandle, imageBufFp16, lenBufFp16, NULL);
        if (retCode != MVNC_OK) {     // error loading tensor
            perror("Could not load ssd tensor\n");
            printf("Error from mvncLoadTensor is: %d\n", retCode);
            return 0;
        }
        // printf("Successfully loaded the tensor for image\n");
        void *resultData16;
        void *userParam;
        unsigned int lenResultData;
        retCode = mvncGetResult(graphHandle, &resultData16, &lenResultData, &userParam);
        if (retCode == MVNC_OK) {
            // Successfully got the result.  The inference result is in the buffer pointed to by resultData
            // convert half precision floats to full floats
            //std::cout << "lenResultData: " << lenResultData << std::endl;
            int numResults = lenResultData / sizeof(half);
            float *resultData32;
            resultData32 = (float *) malloc(numResults * sizeof(*resultData32));
            fp16tofloat(resultData32, (unsigned char *) resultData16, numResults);

            //resultData32 = array_transform(resultData32,13,13,numResults);
            std::vector <Box> resultBoxes;
            //std::cout << "numResults: " << numResults << std::endl;
            //std::cout << "resultData32: "<< resultData32[0] << std::endl;
            SSD_result_infer(resultData32, resultBoxes, buff_[(buffIndex_ + 2) % 3]); //display_img上带框的结果

            free(resultData32);
            //std::cout << "enableConsoleOutput_: " << enableConsoleOutput_ << std::endl;
            if (enableConsoleOutput_) {
                               printf("FPS:%.1f\n", fps_);
//                printf("Objects:\n\n");
            }

            // extract the bounding boxes and send them to ROS
            // iterate through possible boxes and collect the bounding boxes
            int count;

            std::vector<std::vector<RosBox_>> roiBoxes_tmp;
            for (int i = 0; i < numClasses_; ++i) {
                std::vector<RosBox_> tmp;
                for (int j = 0; j < resultBoxes.size(); ++j)
                {
                    if (resultBoxes[j].label == i)
                    {
                        //std::cout << "1111" << std::endl;
                        RosBox_ roiBox;
                        float x_min = resultBoxes[j].x;
                        float y_min = resultBoxes[j].y;
                        float BoundingBox_width = resultBoxes[j].width;
                        float BoundingBox_height = resultBoxes[j].height;
                       // std::cout << "x_min: " << x_min << " y_min: " << y_min << " width: " << BoundingBox_width << " height: " << BoundingBox_height << std::endl;
                        if (BoundingBox_width > 0.01 && BoundingBox_height > 0.01) {
                            //std::cout << "2222" << std::endl;
                            roiBox.x = x_min;
                            roiBox.y = y_min;
                            roiBox.w = BoundingBox_width;
                            roiBox.h = BoundingBox_height;
                            roiBox.Class = resultBoxes[j].label;
                            roiBox.prob = resultBoxes[j].prob;
                            tmp.push_back(roiBox);
                        }
                    }
                }
                //std::cout << "tmp: " << tmp.size() << std::endl;
                //std::cout << "rosBoxCounter_[i]: " << i <<  ", " << tmp.size() << std::endl;
                rosBoxCounter_[i] = tmp.size();
                roiBoxes_tmp.push_back(tmp);
                tmp.clear();
            }

            mutexRoiBoxes_.lock();
            roiBoxes_.clear();
            roiBoxes_ = roiBoxes_tmp;
            mutexRoiBoxes_.unlock();



//            if (resultBoxes.size() > 0) {
////                cameraRect cr;
////                std::vector <cameraRect> human_rects;
////                std::vector <cameraRect> face_rects;
//                for (int k = 1; k < numClasses_; k++) { //对每一类
//                    for (int j = 0; j < resultBoxes.size(); j++) {
//                        if (resultBoxes[j].label == k) {
//                            int xmin = resultBoxes[j].x;
//                            int ymin = resultBoxes[j].y;
//                            int width = resultBoxes[j].width;
//                            int height = resultBoxes[j].height;
////                            cr.rect = cv::Rect(xmin, ymin, width, height);
////                            cr.label = resultBoxes[j].label;
////                            if (k == 1)
////                                human_rects.push_back(cr);
////                            if (k == 2)
////                                face_rects.push_back(cr);
//                        }
//                    }
//                }
//
//                //NMS(human_rects);
//                //NMS(face_rects);
//                // std::cout << "frameWidth_: " << frameWidth_ << " frameHeight_: "<< frameHeight_<< std::endl;
////                mutexCameraRects.lock();
////                Rects_person() = human_rects;
////                Rects_face() = face_rects;
////                mutexCameraRects.unlock();
//                //std::cout << "human_rects: " << human_rects.size() << std::endl;
//                //std::vector<int> ids = select_humans(human_rects, use_time);
//                //mutexSelectId.lock();
//                //selectIds() = ids;
//                //mutexSelectId.unlock();
////                std::cout << "ids.size: " << ids.size() << std::endl;
////                std::cout << "selectIds().size(): " << selectIds().size() << std::endl;
//                // ROS_INFO("Has detection results!");
//                //draw_laser_box(display, laser_rects.size(), laser_rects);
//            } else {
////                mutexCameraRects.lock();
////                Rects_person().clear();
////                Rects_face().clear();
////                mutexCameraRects.unlock();
//            }

            // create array to store found bounding boxes

            demoIndex_ = (demoIndex_ + 1) % demoFrame_;
            running_ = 0;
            delete imageBufFp16;
            return 0;
        }

    }


    void SSD_Detector::ssd() {
        const auto wait_duration = std::chrono::milliseconds(2000);
        //等待image
        while (!getImageStatus()) {
            printf("Waiting for image.\n");
            if (!isNodeRunning()) {
                return;
            }
            std::this_thread::sleep_for(wait_duration);
        }

        std::thread detect_thread;
        std::thread fetch_thread;

        srand(2222222);

        //std::cout << "ros_img: cv " << std::endl;
        cv::Mat ROS_img = getCVImage();
        //std::cout << "ssd ROS_img: " << ROS_img.cols << std::endl;
        buff_[0] = ROS_img.clone();
        buff_[0].copyTo(buff_[1]);
        buff_[0].copyTo(buff_[2]);

        buff_original[0] = ROS_img.clone();
        buff_original[0].copyTo(buff_original[1]);
        buff_original[0].copyTo(buff_original[2]);
       // std::cout << "buff_[0]: " << buff_[0] << std::endl;

        buff_time[0] = ros::Time::now();
        buff_time[1] = buff_time[0];
        buff_time[2] = buff_time[0];
        use_time = buff_time[0];
        //ipl_ = cvCreateImage(cvSize(buff_[0].w, buff_[0].h), IPL_DEPTH_8U, buff_[0].c);

        int count = 0;

        if (!demoPrefix_ && viewImage_) {
            cvNamedWindow("Demo", CV_WINDOW_NORMAL);
            if (fullScreen_) {
                cvSetWindowProperty("Demo", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
            } else {
                cvMoveWindow("Demo", 0, 0);
                cvResizeWindow("Demo", 1352, 1013);
            }
        }

        demoTime_ = getWallTime();
        //std::cout << "demoDone_: " << demoDone_ << std::endl;
        while (!demoDone_) {
            //std::cout << "inside" << std::endl;
            buffIndex_ = (buffIndex_ + 1) % 3;
            fetch_thread = std::thread(&SSD_Detector::fetchInThread, this);
            detect_thread = std::thread(&SSD_Detector::detectInThread, this);

            if (count % (demoDelay_ + 1) == 0) {
                fps_ = 1. / (getWallTime() - demoTime_);
                demoTime_ = getWallTime();
            }

            publishInThread();


            fetch_thread.join();
            detect_thread.join();
            ++count;
            if (!isNodeRunning()) {
                demoDone_ = true;
            }
        }
    }

    cv::Mat SSD_Detector::getCVImage() {
       // std::cout << "getCVImage" << std::endl;
        boost::shared_lock <boost::shared_mutex> lock(mutexImageCallback_);
        cv::Mat ROS_img;
        ROS_img = camImageCopy_;
        //camImageCopy_.copyTo(ROS_img);
       // camImageCopy_.release();

        return ROS_img;
    }

    bool SSD_Detector::getImageStatus(void) {
        boost::shared_lock <boost::shared_mutex> lock(mutexImageStatus_);
        return imageStatus_;
    }

    bool SSD_Detector::isNodeRunning(void) {
        boost::shared_lock <boost::shared_mutex> lock(mutexNodeStatus_);
        return isNodeRunning_;
    }

    void *SSD_Detector::publishInThread() {
        // Publish image.

        //std::cout << "publishInThread" << std::endl;
        cv::Mat cvImage = buff_[(buffIndex_ + 1) % 3];

        std::vector<std::vector<RosBox_>> roiBoxes_temp;
        //Publish boundingBox
        mutexRoiBoxes_.lock();
        roiBoxes_temp = roiBoxes_;
        mutexRoiBoxes_.unlock();


        int total_count = 0;
        for (int i = 0; i < numClasses_; i++) {
            total_count = rosBoxCounter_[i] + total_count;
        }
        if(total_count > 0) {
            //std::cout << "total_count: " << total_count << std::endl;

            std_msgs::Int8 msg;
            msg.data = total_count;
            objectPublisher_.publish(msg);


            // Publish bounding boxes and detection result.
            for (int i = 1; i < numClasses_; i++) { //对每一类
                if (rosBoxCounter_[i] > 0) { //如果有框
                    ssddetect::BoundingBox boundingBox;
                    //把每个框放进来
                    for (int j = 0; j < rosBoxCounter_[i]; j++) {
                       // std::cout << "roiBoxes_ size: " << roiBoxes_.size() << ", i " << roiBoxes_[i].size() << std::endl;
                        int xmin = roiBoxes_temp[i][j].x;
                        int ymin = roiBoxes_temp[i][j].y;
                        int width = roiBoxes_temp[i][j].w;
                        int height = roiBoxes_temp[i][j].h;
                        //std::cout << "aaaaaa" << std::endl;
                        int label_idx = roiBoxes_temp[i][j].Class;
                        boundingBox.Class = classLabels_[label_idx];
                        boundingBox.probability = roiBoxes_temp[i][j].prob;
                        boundingBox.xmin = xmin;
                        boundingBox.ymin = ymin;
                        boundingBox.width = width;
                        boundingBox.height = height;
                        boundingBoxesResults_.boundingBoxes.push_back(boundingBox);

                        cv::Rect box;
                        box.x = xmin;
                        box.y = ymin;
                        box.width = width;
                        box.height = height;

//
//                        cv::rectangle(cvImage, box, cv::Scalar(0,0,0), 2);
//                        cv::putText(cvImage, "person",
//                                    cv::Point(box.x, box.y),
//                                    cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0,0,0), 1, CV_AA);


                    }
                }
            }
            //std::cout << "ddddd" << std::endl;
//            boundingBoxesResults_.header.stamp = ros::Time::now();
//            boundingBoxesResults_.header.frame_id = "detection";
//            boundingBoxesPublisher_.publish(boundingBoxesResults_);

        }else{
            std_msgs::Int8 msg;
            msg.data = 0;
            objectPublisher_.publish(msg);
        }


        boundingBoxesResults_.header.stamp = ros::Time::now();
        boundingBoxesResults_.header.frame_id = "detection";
        boundingBoxesPublisher_.publish(boundingBoxesResults_);


        boundingBoxesResults_.boundingBoxes.clear();
        for (int i = 0; i < numClasses_; i++) {
//            rosBoxes_[i].clear();
            rosBoxCounter_[i] = 0;
        }


        if (!publishDetectionImage(cvImage)) {
            ROS_DEBUG("Detection image has not been broadcasted.");
        }

//        rosBoxes_.clear();


        return 0;
    }

}
