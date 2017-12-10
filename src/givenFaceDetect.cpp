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

bool startFaceDetection = false;
bool foundFace = false;

void startHandler(const std_msgs::Bool::ConstPtr& msg){
    ROS_INFO("Received start message");
    startFaceDetection = true;
}

/** @function main */
int main( int argc, char** argv )
{

    ros::init(argc, argv, "FaceDetection");

    ros::NodeHandle n;

    ros::Publisher unlock_pub = n.advertise<std_msgs::Bool>("unlock", 1000);
    ros::Subscriber start_sub = n.subscribe<std_msgs::Bool>("startFaceDetection", 1000, startHandler);


    // CommandLineParser parser(argc, argv,
    //     "{help h||}"
    //     "{face_cascade|../../data/haarcascades/haarcascade_frontalface_alt.xml|}"
    //     "{eyes_cascade|../../data/haarcascades/haarcascade_eye_tree_eyeglasses.xml|}");

    // cout << "\nThis program demonstrates using the cv::CascadeClassifier class to detect objects (Face + eyes) in a video stream.\n"
    //         "You can use Haar or LBP features.\n\n";
    // parser.printMessage();

    face_cascade_name = "/opt/ros/kinetic/share/OpenCV-3.2.0-dev/haarcascades/haarcascade_frontalface_default.xml";
    eyes_cascade_name = "/opt/ros/kinetic/share/OpenCV-3.2.0-dev/haarcascades/haarcascade_eye.xml";

    VideoCapture capture;
    Mat frame;

    //-- 1. Load the cascades
    if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading face cascade\n"); return -1; };
    if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading eyes cascade\n"); return -1; };

    ROS_INFO("Opening the Camera");

    //-- 2. Read the video stream
    capture.open( 0 );
    if ( ! capture.isOpened() ) { printf("--(!)Error opening video capture\n"); return -1; }

    while(ros::ok()){
        ros::spinOnce();
        if(startFaceDetection){
            capture.read(frame);
            if( frame.empty() ){
                printf(" --(!) No captured frame -- Break!");
                break;
            }

            detectAndDisplay( frame );

            if(foundFace){
                ROS_INFO("Found a Face");
                std_msgs::Bool msg;
                msg.data = true;
                unlock_pub.publish(msg);
                startFaceDetection = false;
                foundFace = false;
                destroyAllWindows();
            } else {
                char c = (char)waitKey(10);
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
    std::vector<Rect> faces;
    Mat frame_gray;

    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 30, 0|CASCADE_SCALE_IMAGE, Size(50, 50) );

    if(faces.size() > 0){
        foundFace = true;
    }

    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

        Mat faceROI = frame_gray( faces[i] );
        
    }
    //-- Show what you got
    if(!foundFace){
        imshow( window_name, frame );    
    }
    
}