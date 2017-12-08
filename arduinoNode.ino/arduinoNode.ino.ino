#include <Ultrasonic.h>
#include <ros.h>
#include <std_msgs/Float64.h>

Ultrasonic ultra(12, 13);
ros::NodeHandle nh;
std_msgs::Float64 rangeReading;
ros::Publisher chatter("range", &rangeReading);

void setup() {
  // put your setup code here, to run once:
  nh.initNode();
  nh.advertise(chatter);

}

void loop() {
  // put your main code here, to run repeatedly:
  int reading = 0;
  if ( (reading = ultra.distanceRead()) != 0.0) {
    rangeReading.data = reading;
    chatter.publish( &rangeReading);
    nh.spinOnce();
    delay(1000);
  }

}
