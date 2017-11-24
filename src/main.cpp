//
// Created by djcupo on 11/22/17.
//

#include <iostream>
#include <time.h>
#include "opencv2/opencv.hpp"
#include "ros/ros.h"
#include "std_msgs/Bool.h"

bool startFaceDetection = false;
bool findingValidFace = true;

void startHandler(const std_msgs::Bool::ConstPtr& msg){
    startFaceDetection = true;
}

int main(int argc, char* argv[]){

    ros::init(argc, argv, "FaceDetection");

    ros::NodeHandle n;

    ros::Publisher unlock_pub = n.advertise<std_msgs::Bool>("unlock", 1000);
    ros::Subscriber start_sub = n.subscribe<std_msgs::Bool>("startFaceDetection", 1000, startHandler);

    ros::Rate loop(10);

    cv::CascadeClassifier classifier = cv::CascadeClassifier("haarcascade_frontalface_default.xml");
    cv::VideoCapture cap = cv::VideoCapture(0);

    int count = 0;
    while( ros::ok() ){
        if(startFaceDetection){

            ROS_INFO("Started Face Detection");

            time_t start = time(0);
            startFaceDetection = false;
            findingValidFace = true;
            while(findingValidFace){

                ROS_INFO("Trying to find a valid face");

                cv::Mat colorFrame, greyFrame;
                if(cap.isOpened()){
                    cap >> colorFrame;

                    cv::cvtColor(colorFrame, greyFrame, CV_BGR2GRAY);

                    std::vector<cv::Rect> foundFaces;

                    classifier.detectMultiScale(greyFrame, foundFaces);

                    if (foundFaces.size() > 0) {
                        std_msgs::Bool msg;
                        msg.data = true;
                        unlock_pub.publish(msg);
                        findingValidFace = false;
                    }

                    if( difftime(time(0), start) > 30){
                        findingValidFace = false;
                    }
                } else {
                    ROS_INFO("Couldn't open the camera");
                    break;
                }
                
            }

        }

        ros::spinOnce();

    }
}