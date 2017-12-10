# -*- coding: utf-8 -*-
"""
Created on Sun Nov 26 13:40:19 2017

@author: Nishant
"""
import random as rand
import  matplotlib.pyplot as plt 
import numpy as np
import math
import rospy
from sensor_msgs.msg import Range
from std_msgs.msg import Bool
from collections import deque

pub = rospy.Publisher('cam_flag', Bool, queue_size=10)
bag=[]
temp=[]
if __name__ == '__main__':
    try:		
     #  	strl = "Filtering node started "
      #  rospy.loginfo(strl)      
        listener()
    except rospy.ROSInterruptException:
           pass


 def talker(flag):
       	   rate = rospy.Rate(10) # 10hz
           pub.publish(flag)
           #rate.sleep()

def callback(data):
	if len(temp)<10:
		temp.append(data.data)
		if len(temp)==10:
			bag=deque(temp)
		return
		
	bag.append(data.data)
	bag.popleft()
	fvs=apply_wma(bag)
	cam=False
	for e in fws:
		if e<38:
			cam=True
	talker(cam)

def listener():
    
       # In ROS, nodes are uniquely named. If two nodes with the same
       # node are launched, the previous one is kicked off. The
       # anonymous=True flag means that rospy will choose a unique
 	     # name for our 'listener' node so that multiple listeners can
       # run simultaneously.
       rospy.init_node('listener', anonymous=True)
   	   
       rospy.Subscriber("range", Range, callback)
   
       # spin() simply keeps python from exiting until this node is stopped
       rospy.spin()
   



def apply_gf(x,y,n=8):
    y2=[]
    win=create_Gaussian_filter(n,2.5)

    for i in range(len(x)):
        w=[]
        if i>=n-1:       
            for t in range(n):
               w.append(win[t]*y[i-n+t+1])
            y2.append(sum(w))
        else:
            y2.append(y[i])
    return y2

def create_Gaussian_filter(n,sig):
    x=range(n)
    x=[i-int(n/2) for i in x]

    def Gau(x,sigm):
        pow=np.exp(-(x*x)/sigm/sigm/2)
        return (pow/np.sqrt(2*math.pi*sigm*sigm))
    x=[Gau(f,sig) for f in x]
    return x/sum(x)
    
    
def apply_wma(y,n=5):
    y2=[]
    

    for i in range(len(y)):
        w=[]
        if i>=n-1:
            for t in range(n):
               w.append((2*(t+1)/n/(n+1))*y[i-t])
            y2.append( sum(w))
        else:
            y2.append(y[i])
    return y2


        
