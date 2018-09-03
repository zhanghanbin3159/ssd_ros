#include <opencv2/opencv.hpp>
#include <signal.h>
#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>
using namespace cv;
using namespace std;
Mat img;
struct Flag{
    bool shutdown = false;
    bool img = false;
} flag;
void img_sub(const sensor_msgs::Image& msg_img); //func to get img
void siginthd_kb(int sig) //func to shutdown this node
{
    cout << "going to shudown!" << endl;
    flag.shutdown = true;
}
int main(int argc, char** argv){
    ros::init(argc, argv, "listener_C", ros::init_options::NoSigintHandler);
    ros::NodeHandle nh;
    signal(SIGINT, siginthd_kb);
    ros::Subscriber img_rcv = nh.subscribe("/tutorial/image", 10, &img_sub);
    char key = ' ';
    cout << "press Esc to exit.\n";
    while (key != 27 && flag.shutdown == false)
    {
        if (flag.img)
        {
            imshow("img", img);
            flag.img = false;
        }
        else
            cout << "img not ready!\n";
        key = waitKey(3);
        ros::spinOnce();
    }
   return 0;
}//end for main
void img_sub(const sensor_msgs::Image &msg_img)
{
    cv_bridge::CvImagePtr cv_ptr;
    cv_ptr = cv_bridge::toCvCopy(msg_img, "bgr8");
    img = cv_ptr->image;
    flag.img = true;
}
