#!/usr/bin/env python
import rospy
from std_msgs.msg import Bool

def talker():
    pub = rospy.Publisher('startFaceDetection', Bool, queue_size=10)
    rospy.init_node('ManualStart', anonymous=True)
    while not rospy.is_shutdown():
        command = raw_input("Press Enter to send start command, or q to exit\n")
        if command is 'q':
            break
        else:
          pub.publish(True)

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass