//Written by Dominic Cupo for RBE500 Hands On Project

//Includes

#include <Ultrasonic.h>

#include <ros.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Bool.h>

//Defines for pins

#define TRIGGER_PIN 12
#define ECHO_PIN 13

#define BUTTON_PIN 2
#define DOOR_UNLOCK_PIN 3

//Sets LED to HIGH value when message comes in
void handleUnlock(const std_msgs::Bool &msg){
  digitalWrite(DOOR_UNLOCK_PIN, HIGH);
}

//Object Instansiations 

//Ultrasonic object with pin assignments
Ultrasonic ultra(TRIGGER_PIN, ECHO_PIN);

//Ros Node handler object
ros::NodeHandle nh;

//Empty Ros messages
std_msgs::Float64 rangeReading;
std_msgs::Bool buttonState;

//Ros Publisher assignments
ros::Publisher chatter("range", &rangeReading);
ros::Publisher button("reset", &buttonState);

//Ros Subscriber for unlock command
ros::Subscriber<std_msgs::Bool> unlock("unlock", &handleUnlock);

void setup() {
  //Set modes for button and LED
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(DOOR_UNLOCK_PIN, OUTPUT);

  //Init the Ros Node
  nh.initNode();

  //Set up subscrivers and publishers
  nh.advertise(chatter);
  nh.advertise(button);
  nh.subscribe(unlock);

}

void loop() {

  //Update topics
  nh.spinOnce();

  //Make sure there is a valid reading, and set it as an msg variable for use
  if ( (rangeReading.data = ultra.distanceRead()) != 0.0) {

    //If valid, publish
    chatter.publish( &rangeReading);
  }
  
  //If the reset button has been pressed:
  if (!digitalRead(BUTTON_PIN)) {
    
    //Publish RESET message
    buttonState.data = true;
    button.publish(&buttonState);
    
    //reset unlocked status
    digitalWrite(DOOR_UNLOCK_PIN, LOW);
  }
  
  //delay half a second so as to not spam the filter node
  delay(500);

}
