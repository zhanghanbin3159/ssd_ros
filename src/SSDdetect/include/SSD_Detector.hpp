#ifndef SSD_OBJECTDETECTOR_HPP
#define SSD_OBJECTDETECTOR_HPP

#pragma once

// c++
#include <math.h>
#include <string>
#include <vector>
#include <iostream>
#include <pthread.h>
#include <thread>
#include <chrono>

// ROS
#include <ros/ros.h>
#include <std_msgs/Int8.h>
#include <actionlib/server/simple_action_server.h>
#include <sensor_msgs/image_encodings.h>
#include <sensor_msgs/Image.h>
#include <geometry_msgs/Point.h>
#include <image_transport/image_transport.h>

// OpenCv
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <cv_bridge/cv_bridge.h>

// msgs
#include <ssddetect/BoundingBoxes.h>
#include <ssddetect/BoundingBox.h>


//ncs
#include <mvnc.h>
#include "ncs_param.h"
#include "image_interface.h"


extern "C" {
#include "fp16.h"
}



// 16 bits.  will use this to store half precision floats since C++ has no
// built in support for it.
typedef unsigned short half;

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

typedef struct{
    int label;
    int x;
    int y;
    int width;
    int height;
    float prob;
} Box;

namespace ssd_ros {
    char *cfg;
    char *GRAPH_FILE_NAME;
    char *data;
    char **detectionNames;


//! Bounding box of the detected object.
    typedef struct {
        float x, y, w, h, prob;
        int num, Class;
    } RosBox_;

    class SSD_Detector {
    public:
        /*!
         * Constructor.
         */
        explicit SSD_Detector(ros::NodeHandle nh);

        /*!
         * Destructor.
         */
        ~SSD_Detector();

    private:
        /*!
         * Reads and verifies the ROS parameters.
         * @return true if successful.
         */
        bool readParameters();

        /*!
         * Initialize the ROS connections.
         */
        void init();

        void init_ncs();

        /*!
         * Callback of camera.
         * @param[in] msg image pointer.
         */
        void cameraCallback(const sensor_msgs::ImageConstPtr &msg);

        /*!
         * Check for objects action goal callback.
         */
        //void checkForObjectsActionGoalCB();

        /*!
         * Check for objects action preempt callback.
         */
        //void checkForObjectsActionPreemptCB();

        /*!
         * Check if a preempt for the check for objects action has been requested.
         * @return false if preempt has been requested or inactive.
         */
        bool isCheckingForObjects() const;

        /*!
         * Publishes the detection image.
         * @return true if successful.
         */
        bool publishDetectionImage(const cv::Mat &detectionImage);

        //! Typedefs.

        //! ROS node handle.
        ros::NodeHandle nodeHandle_;

        //! Class labels.
        int numClasses_;
        std::vector <std::string> classLabels_;

        //! Check for objects action server.
        //CheckForObjectsActionServerPtr checkForObjectsActionServer_;

        //! Advertise and subscribe to image topics.
        image_transport::ImageTransport imageTransport_;

        //! ROS subscriber and publisher.
        image_transport::Subscriber imageSubscriber_;
        ros::Publisher objectPublisher_;
        ros::Publisher boundingBoxesPublisher_;

        //! Detected objects.
        std::vector <std::vector<RosBox_>> rosBoxes_;
        std::vector<int> rosBoxCounter_;
        ssddetect::BoundingBoxes boundingBoxesResults_;

        std::vector<cv::Rect> laser_rects;

        //! Camera related parameters.
        int frameWidth_;
        int frameHeight_;

        //! Publisher of the bounding box image.
        ros::Publisher detectionImagePublisher_;

        // SSD running on thread.
        std::thread ssdThread_;

        std::mutex mutexRoiBoxes_;


        // SSD.
        char **demoNames_;
        image **demoAlphabet_;
        int demoClasses_;

        float **probs_;
//        box *boxes_;
//        network net_;
        cv::Mat buff_[3];
        cv::Mat buff_original[3];
        ros::Time buff_time[3];
//        image buffLetter_[3];
        int buffId_[3];
        int buffIndex_ = 0;
        IplImage *ipl_;
        float fps_ = 0;
        float demoThresh_ = 0;
        float demoHier_ = .5;
        int running_ = 0;

        int demoDelay_ = 0;
        int demoFrame_ = 3;
        int demoDetections_ = 0;
        float **predictions_;
        int demoIndex_ = 0;
        int demoDone_ = 0;
        float *lastAvg2_;
        float *lastAvg_;
        float *avg_;
        double demoTime_;

        std::vector<std::vector<RosBox_>> roiBoxes_;
        ros::Time use_time;
        bool viewImage_;
        bool enableConsoleOutput_;
        int waitKeyDelay_;
        int fullScreen_;
        char *demoPrefix_;

        float ssd_threshold;

        cv::Mat camImageCopy_;
        boost::shared_mutex mutexImageCallback_;

        bool imageStatus_ = false;
        boost::shared_mutex mutexImageStatus_;

        bool isNodeRunning_ = true;
        boost::shared_mutex mutexNodeStatus_;

        int actionId_;
        boost::shared_mutex mutexActionStatus_;

        double getWallTime();

        void *fetchInThread();

        void *detectInThread();

       // void *displayInThread(void *ptr);

        //void *displayLoop(void *ptr);

       // void *detectLoop(void *ptr);

        void SSD_result_infer(float* output, std::vector<Box>& result, cv::Mat& image);

        bool Overlay_on_image(cv::Mat& image, float* object_info, int Length, Box& single_box);

//        void setupNetwork(char *cfgfile, char *datafile, float thresh,
//                          char **names, int classes,
//                          int delay, char *prefix, int avg_frames, float hier, int w, int h,
//                          int frames, int fullscreen);
        //std::vector<int> select_humans(std::vector<cameraRect> Rects_camera, ros::Time time_u);

        void ssd();

        cv::Mat getCVImage();

        bool getImageStatus(void);

        bool isNodeRunning(void);

        void *publishInThread();

//        void NMS(std::vector <cameraRect> &M);
    };
}
#endif
