#ifndef __Camera_ziwei
#define __Camera_ziwei

#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <cv.h>
#include <highgui.h>
#include <cv_bridge/cv_bridge.h>
#include "human.h"
#include <algorithm>
#include <Eigen/Dense>
#include <ros/ros.h>



cv::Mat& cameraFrame();
int& frameHeight();
int& frameWidth();
int& frameDiag();




void cameraCallback(const sensor_msgs::Image::ConstPtr &image);

bool projectBoxInCamera(Human &human);
bool adjustOutBox(cv::Rect &rect, int cols, int rows);
std::pair<double,double> ReProject(cv::Rect A, Human hum);
std::pair<double,double> ReProject(cv::Rect A);



#endif