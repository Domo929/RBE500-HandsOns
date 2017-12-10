import random as rand
import  matplotlib.pyplot as plt 
import numpy as np
import math
import rospy
from scipy.signal import savgol_filter
from sensor_msgs.msg import Range
from std_msgs.msg import Bool


pub = rospy.Publisher('cam_flag', Bool, queue_size=10)
bag=[]

def talker(flag):
           pub.publish(flag)

def callback(data):
	  if rospy.is_shutdown():
		  if len(bag)<10:
		    bag.append(data)
		    return

		  bag.append(data)
		  bag.pop(0)
		  
		  #### SELECT TYPE OF FILTER ####
		  #Moving av filter
		  fvs=apply_wma(bag) 

		  #Savitzky Golay filter
		  #fvs = SG_fil(bag)

		  cam=False

		  if fvs<38:
		      cam=True

		  print(cam)
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
    
    
def apply_wma(y):
    s=0
    n=len(y)
    for i in range(n):
	  #print(2*y[i]*(i+1)/n/(n+1))
          s=s+2*y[i]*(i+1)/n/(n+1)
    return s


#SG Filter function block
def SG_fil(data):
	filter_val = savgol_filter(data,7,2)
	mid_index = (len(filter_val) - 1)/2
	final_val = filter_val[mid_index]
    	return final_val


if __name__ == '__main__':
    try:		
      strl = "Filtering node started "
      rospy.loginfo(strl)      
      listener()
    except rospy.ROSInterruptException:
           pass


