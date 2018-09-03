#include "SSD_Detector.hpp"

int main(int argc, char *argv[]) {
//
    ros::init(argc, argv, "ssddetect");
    ros::NodeHandle nh, priv_nh("~");

    ssd_ros::SSD_Detector ssd_Detector(priv_nh);
    ros::spin();
}