#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "ros/ros.h"
#include "std_msgs/Bool.h"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay( Mat frame );

/** Global variables */
String face_cascade_name, eyes_cascade_name;
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
String window_name = "Capture - Face detection";

// trigger bools
bool startFaceDetection = false;
bool foundFace = false;

// handler for the start message
void startHandler(const std_msgs::Bool::ConstPtr& msg){
    ROS_INFO("Received start message");
    startFaceDetection = true;
}

/** @function main */
int main( int argc, char** argv )
{
    //init the ROS node with name "FaceDetection"
    ros::init(argc, argv, "FaceDetection");

    //Pub and Sub handler
    ros::NodeHandle n;

    //Publisher for unlock command
    ros::Publisher unlock_pub = n.advertise<std_msgs::Bool>("unlock", 1000);
    
    //Subscriber for start trigger
    ros::Subscriber start_sub = n.subscribe<std_msgs::Bool>("cam_flag", 1000, startHandler);


    //Hardcoded path for Haar cascade XML
    face_cascade_name = "/opt/ros/kinetic/share/OpenCV-3.2.0-dev/haarcascades/haarcascade_frontalface_default.xml";
    eyes_cascade_name = "/opt/ros/kinetic/share/OpenCV-3.2.0-dev/haarcascades/haarcascade_eye.xml";

    //Video Capture object for accessing Webcam
    VideoCapture capture;

    //Image storage for each frame of the video
    Mat frame;

    //-- 1. Load the cascades
    if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading face cascade\n"); return -1; };
    if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading eyes cascade\n"); return -1; };


    ROS_INFO("Starting FaceDetection Node");

    //-- 2. Read the video stream
    capture.open( 0 );
    if ( ! capture.isOpened() ) { printf("--(!)Error opening video capture\n"); return -1; }

    //While were not shutdown
    while(ros::ok()){

        //Check subscribers and publish
        ros::spinOnce();

        //If we have recieved the start command
        if(startFaceDetection){

            //Pull the image frame
            capture.read(frame);

            //If the frame is empty, print and state that, then exit
            if( frame.empty() ){
                ROS_INFO(" --(!) No captured frame -- Break!");
                break;
            }

            //Run detect and display
            detectAndDisplay( frame );

            //If we found a face
            if(foundFace){

                //Print that we found one
                ROS_INFO("Found a Face");

                //Create the message, fill it, and publish it for next ROS Spin
                std_msgs::Bool msg;
                msg.data = true;
                unlock_pub.publish(msg);

                //End face detection block
                startFaceDetection = false;

                //Restart found face trigger
                foundFace = false;

                //Close the imshow windows
                destroyAllWindows();

            // Otherwise, if we haven't found a face
            } else {

                //Store the char used in waitkey
                char c = (char)waitKey(10);

                //if 'ESC' key, or ROS is closed, exit and break from the loop
                if( c == 27  || !ros::ok()) {
                   break; 
               }
           }

       }
   }
   return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame )
{
    //Store the vecor of faces
    std::vector<Rect> faces;

    //Extra image holder for grey conversion
    Mat frame_gray;

    //Convert to grey, and equalize brightness
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 30, 0|CASCADE_SCALE_IMAGE, Size(50, 50) );

    //If we found a face, set trigger
    if(faces.size() > 0){
        foundFace = true;
    }


    //Show what you got so people can see if they're hiding
    if(!foundFace){
        imshow( window_name, frame );    
    }
    
}