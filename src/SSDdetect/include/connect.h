#ifndef __CONNECT_ziwei
#define __CONNECT_ziwei

#include "ncs_param.h"
#include "camera.h"
#include <Eigen/Dense>

#define Buff_size 4

//std::mutex mutexLaserSet;
//class id_finder
//{
//public:
//    id_finder( int l ) : id(l) {}
//    bool operator () (const Human human)
//    {
//        return human.getId() == id;
//    }
//private:
//    int id;
//};
float Kdist(cv::Rect A, cv::Rect B);

typedef struct
{
    cv::Rect rect;
    int label; //框里目标所属类别
}cameraRect;

typedef struct
{
    cv::Rect rect;
    int id;
}laserRect;

typedef struct{
    std::vector<laserRect> laser_rects;
    std::vector<Human> laser_humans;
    ros::Time timestamp;
} laserSet;


typedef struct{
    int laser_id;
    //first: CameraRects()的idx，second: 对应overlap
    std::vector<std::pair<int,float>> idx_overlap;
    int max_overlap_idx;
    float max_overlap;
} Camera_rect_Info;

class max_idx_finder
{
public:
    max_idx_finder( int l ) : max_idx(l) {}
    bool operator () (const Camera_rect_Info A)
    {
        return A.max_overlap_idx == max_idx;
    }
private:
    int max_idx;
};

std::vector<std::vector<Camera_rect_Info>::iterator>::iterator
max_overlap_element(std::vector<std::vector<Camera_rect_Info>::iterator> group);


//float getOverlap(const cv::Rect &b1, const cv::Rect &b2);
std::vector<Human> &cameraHumans(); //从camera frame回来通过了Human detection 的human
//std::vector<Human> &laserHumans(); //从laser frame 传给camera做human detection的human
std::deque<laserSet> &Laser_sets();
//std::vector<laserRect> &Rects_laser();
std::vector<cameraRect> &Rects_person();
std::vector<cameraRect> &Rects_face();

std::vector<int> &selectIds();
void laser_human_extraction(std::list<Human> &humans);
//std::mutex &mutexLock();


#endif