/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\Student                                          */
/*    Created:      Mon Feb 08 2021                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// leftfront            motor         1               
// leftback             motor         2               
// rightfront           motor         3               
// rightback            motor         4               
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

//global variables
float rampUpSpeed = 0.5;
float rampDownSpeed = 0.9;

float currentSpeedX = 0;
float currentSpeedY = 0;

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
  while(true){
    //get controller input
    float inX = Controller1.Axis4.value() / 100;
    float inY = Controller1.Axis3.value() / 100;

    //x first
    float currentSpeedX = ((inX > currentSpeedX) ? (inX>0 ? currentSpeedX+=rampUpSpeed : currentSpeedX-=rampUpSpeed) : (inX>0 ? currentSpeedX-=rampUpSpeed : currentSpeedX+=rampUpSpeed));
    float currentSpeedY = ((inY > currentSpeedY) ? (inY>0 ? currentSpeedY+=rampUpSpeed : currentSpeedY-=rampUpSpeed) : (inY>0 ? currentSpeedY-=rampUpSpeed : currentSpeedY+=rampUpSpeed));

    leftfront.setVelocity((currentSpeedX - currentSpeedY)*100, vex::velocityUnits::pct);
    leftback.setVelocity((currentSpeedY - currentSpeedX)*100, vex::velocityUnits::pct);
    rightfront.setVelocity((currentSpeedY - currentSpeedX)*100, vex::velocityUnits::pct);
    rightback.setVelocity((currentSpeedY + currentSpeedX)*100, vex::velocityUnits::pct);

    leftfront.spin(forward);
    leftback.spin(forward);
    rightfront.spin(forward);
    rightback.spin(forward);
  }
}
