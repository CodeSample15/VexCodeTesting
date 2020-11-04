/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\lukec                                            */
/*    Created:      Tue Nov 03 2020                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// inertia              inertial      1               
// vertencoder          rotation      2               
// strafeencoder        rotation      3               
// leftfront            motor         4               
// leftback             motor         5               
// rightfront           motor         6               
// rightback            motor         7               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "math.h"

using namespace vex;

int xPos = 0;
int yPos = 0;
int yawValue = 0;

void moveTo(int x, int y) {
  //calculating motor speeds and direction
  double yawModifier = 1; //this is for the inertial sensor not giving the right numbers to be fed into the system

  double xValue = cos(yawValue / yawModifier) + (x - xPos);
  double yValue = sin(yawValue / yawModifier) + (y - yPos);

  //applying those values to the motors
  double frontLeft = (double)(yValue + xValue * .5);
  double backLeft = (double)(yValue - xValue * .5);
  double frontRight = (double)(yValue - xValue * .5);
  double backRight = (double)(yValue + xValue * .5);

  vertencoder.setPosition(0, degrees);
  strafeencoder.setPosition(0, degrees);

  leftfront.setVelocity(frontLeft, vex::velocityUnits::pct);
  leftback.setVelocity(backLeft, vex::velocityUnits::pct);
  rightfront.setVelocity(frontRight, vex::velocityUnits::pct);
  rightback.setVelocity(backRight, vex::velocityUnits::pct);

  while(abs(xPos-x) < 2 && abs(yPos-y) < 2) {
    leftfront.spin(forward);
    leftback.spin(forward);
    rightfront.spin(forward);
    rightback.spin(forward);
  }

  xPos += strafeencoder.position(degrees);
  yPos += vertencoder.position(degrees);

  leftfront.stop();
  leftback.stop();
  rightfront.stop();
  rightback.stop();
}

void rightinertialturn(double goaldegrees)
{

  inertia.calibrate();
  while (inertia.isCalibrating()) {
    wait(.3, seconds);
  }

  leftfront.setVelocity(40, vex::velocityUnits::pct);
  leftback.setVelocity(40, vex::velocityUnits::pct);
  rightfront.setVelocity(40, vex::velocityUnits::pct);
  rightback.setVelocity(40, vex::velocityUnits::pct);
    
  while(inertia.rotation(degrees) < goaldegrees)
  {
    leftfront.spin(forward);
    leftback.spin(forward);
    rightfront.spin(reverse);
    rightback.spin(reverse);
  }

  yawValue += inertia.rotation(degrees); //position tracking

  leftfront.stop();
  leftback.stop();
  rightfront.stop();
  rightback.stop();

  wait (1,seconds);
}

void leftinertialturn(double goaldegrees)
{
  inertia.calibrate();
  while (inertia.isCalibrating()) {
    wait(.3, seconds);
  }

  leftfront.setVelocity(40, vex::velocityUnits::pct);
  leftback.setVelocity(40, vex::velocityUnits::pct);
  rightfront.setVelocity(40, vex::velocityUnits::pct);
  rightback.setVelocity(40, vex::velocityUnits::pct);

  while(inertia.rotation(degrees) > goaldegrees)
  {
    leftfront.spin(reverse);
    leftback.spin(reverse);
    rightfront.spin(forward);
    rightback.spin(forward);
  }

  yawValue += inertia.rotation(degrees); //position tracking

  leftfront.stop();
  leftback.stop();
  rightfront.stop();
  rightback.stop();

  wait (1,seconds);
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
}