//
// Created by djcupo on 11/22/17.
//

#include <iostream>
#include "opencv2/opencv.hpp"
#include "ros/ros.h"
#include "std_msgs/String.h"

int main(int argc, char* argv[]){

    ros::init(argc, argv, "FaceDetection");

    ros::NodeHandle n;

    ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);

    ros::Rate loop(10);

    int count = 0;
    while(ros::ok()){
        std_msgs::String msg;

        std::stringstream ss;
        ss << "hello world " << count;
        msg.data = ss.str();

        ROS_INFO("%s", msg.data.c_str());

        /**
         * The publish() function is how you send messages. The parameter
         * is the message object. The type of this object must agree with the type
         * given as a template parameter to the advertise<>() call, as was done
         * in the constructor above.
         */
        chatter_pub.publish(msg);

        ros::spinOnce();

        count++;

        loop.sleep();
    }
}