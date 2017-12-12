#!/usr/bin/env python

import random as rand
import matplotlib.pyplot as plt
import numpy as np
import math
import rospy

from std_msgs.msg import Float64
from std_msgs.msg import Bool

bag = []




def talker(flag):
    global pub
    if flag:
    	pub.publish(flag)
    	rospy.sleep(5)

def resetCallback(msg):
	global reset
	reset = msg.data


def callback(msg):
    if not rospy.is_shutdown():

        if len(bag) < 5:
            bag.append(msg.data)
            return

        bag.append(msg.data)
        bag.pop(0)
        fvs = apply_wma(bag)
        cam = False

        if fvs < 38:
            cam = True

        print(cam)
        print(rospy.get_rostime().secs)
        talker(cam)


def apply_gf(x, y, n=8):
    y2 = []
    win = create_Gaussian_filter(n, 2.5)

    for i in range(len(x)):
        w = []
        if i >= n - 1:
            for t in range(n):
                w.append(win[t] * y[i - n + t + 1])

            y2.append(sum(w))

        else:
            y2.append(y[i])

    return y2


def create_Gaussian_filter(n, sig):
    x = range(n)
    x = [i - int(n / 2) for i in x]

    def Gau(x, sigm):
        pow = np.exp(-(x * x) / sigm / sigm / 2)
        return (pow / np.sqrt(2 * math.pi * sigm * sigm))

    x = [Gau(f, sig) for f in x]
    return x / sum(x)


def apply_wma(y):
    s = 0
    n = len(y)
    for i in range(n):
        # print(2*y[i]*(i+1)/n/(n+1))
        s = s + 2 * y[i] * (i + 1) / n / (n + 1)
    return s


if __name__ == '__main__':
    try:
        global subRange
        global pub
        strl = "Filtering node started "
        rospy.loginfo(strl)

        rospy.init_node('Filter')

        subRange = rospy.Subscriber("range", Float64, callback)
        subReset = rospy.Subscriber("reset", Bool, resetCallback)
        pub = rospy.Publisher('cam_flag', Bool, queue_size=10)

        rospy.spin()

    except rospy.ROSInterruptException:
        pass
