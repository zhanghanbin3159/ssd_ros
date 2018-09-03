#!/usr/bin/env python

import os
import time
import numpy
import skimage.io
import vis
import skimage.transform
from PIL import Image
import mvnc.mvncapi as mvnc
import matplotlib.pyplot as plt
import rospy
from sensor_msgs.msg import Image
import cv2
from cv_bridge import CvBridge


# input parameters
IMAGE_MEAN = [71.60167789, 82.09696889, 72.30608881]
GRAPH_PATH = '/home/pesong/GS_catkin_ws/src/beginner_tutorials/graph/Inception_fcn4s_city.graph'
IMAGE_DIM = [320, 480]

# --------step1: open the device and get a handle to it--------------------
# look for device
devices = mvnc.EnumerateDevices()
if len(devices) == 0:
    print("No devices found")
    quit()

device = mvnc.Device(devices[0])
device.OpenDevice()

# ---------step2: load a graph file into hte ncs device----------------------
# read the graph file into a buffer
with open(GRAPH_PATH, mode='rb') as f:
    blob = f.read()

# Load the graph buffer into the ncs
graph = device.AllocateGraph(blob)



def callback(imgmsg):

    # print(imgmsg)
    bridge = CvBridge()
    img_ori = bridge.imgmsg_to_cv2(imgmsg, "bgr8")

    # Resize image [Image size is defined during training]
    img_resize = skimage.transform.resize(img_ori, IMAGE_DIM) * 255

    # Convert RGB to BGR [skimage reads image in RGB, some networks may need BGR]
    image_t = img_resize[:, :, ::-1]

    # Mean subtraction & scaling [A common technique used to center the data]
    image_t = image_t.astype(numpy.float16)
    image_t = (image_t - numpy.float16(IMAGE_MEAN))

# -----------step4: get result-------------------------------------------------
    graph.LoadTensor(image_t, 'user object')

    # Get the results from NCS
    out = graph.GetResult()[0]

    #  flatten ---> image
    out = out.reshape(-1, 2).T.reshape(2, 331, -1)
    out = out.argmax(axis=0)
    out = out[:-11, :-11] 
    
    # save result
    voc_palette = vis.make_palette(2)

    # get masked image
    img_masked = vis.vis_seg(img_resize, out, voc_palette)

    # # visualization
    # cv2.imshow("ori", img_ori)
    cv2.imshow("lib", img_masked)
    cv2.waitKey(3)

def listener():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # node are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.

    rospy.init_node('listener', anonymous=True)
    rospy.Subscriber("/tutorial/image", Image, callback)
    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    listener()
