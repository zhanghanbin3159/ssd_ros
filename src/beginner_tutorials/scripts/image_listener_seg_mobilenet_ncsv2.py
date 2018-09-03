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
GRAPH_PATH = '/home/ziwei/GS_catkin_ws/src/beginner_tutorials/graph/MobileNetV2_60000.graph'
#GRAPH_PATH = '/home/pesong/catkin_ws/src/beginner_tutorials/graph/mobilenetv2_fcn4s_ncsV2.graph'
IMAGE_DIM = [320, 480]

# configure the NCS
# ***************************************************************
mvnc.global_set_option(mvnc.GlobalOption.RW_LOG_LEVEL, 2)

# --------step1: open the device and get a handle to it--------------------
# look for device
devices = mvnc.enumerate_devices()
if len(devices) == 0:
    print("No devices found")
    quit()

# Pick the first stick to run the network
device = mvnc.Device(devices[0])

# Open the NCS
device.open()

# The graph file that was created with the ncsdk compiler
graph_file_name = 'graph'


# ---------step2: load a graph file into hte ncs device----------------------
# Load network graph file into memory
with open(graph_file_name, mode='rb') as f:
    blob = f.read()

# create and allocate the graph object
graph = mvnc.Graph('graph')
fifo_in, fifo_out = graph.allocate_with_fifos(device, blob)



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
    graph.queue_inference_with_fifo_elem(fifo_in, fifo_out, image_t, 'user object')

    # Get the results from NCS
    out, userobj = fifo_out.read_elem()

    #  flatten ---> image
    out = out.reshape(-1, 2).T.reshape(2, 331, -1)
    out = out.argmax(axis=0)
    out = out[6:-5, 6:-5]

    # save result
    voc_palette = vis.make_palette(2)
    #out_im = Image.fromarray(vis.color_seg(out, voc_palette))
    #iamge_name = IMAGE_PATH.split('/')[-1].rstrip('.jpg')
    # out_im.save('demo_test/' + iamge_name + '_ncs_' + '.png')

    # get masked image
    img_masked = Image.fromarray(vis.vis_seg(img_ori, out, voc_palette))

    # # visualization
    img_ori = img_ori[:, :, ::-1]
    img_masked = img_masked[:, :, ::-1]
    cv2.imshow("in", img_ori)
    cv2.imshow("out", img_masked)
    cv2.waitKey(1)

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
