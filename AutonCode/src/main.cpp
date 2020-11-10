  
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
// leftfront            motor         13              
// leftback             motor         8               
// rightfront           motor         5               
// rightback            motor         12              
// inertia              inertial      7               
// vertencoder          encoder       A, B            
// strafeencoder        encoder       C, D            
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

int xPos = 0;
int yPos = 0;
int yawValue = 0;
int yG = 0;
int xG = 0;

bool moving = false;

void display() {
  while(true) {
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Vertencoder: %f", vertencoder.position(degrees));

    Brain.Screen.setCursor(2, 1);
    Brain.Screen.print("Strafeencoder: %f", strafeencoder.position(degrees));

    Brain.Screen.setCursor(3, 1);
    Brain.Screen.print("Is moving: %f", moving);

    Brain.Screen.setCursor(4, 1);
    Brain.Screen.print("yG: %d", yG);

    Brain.Screen.setCursor(5, 1);
    Brain.Screen.print("xG: %d", xG);

    wait(15, msec);
    Brain.Screen.clearScreen();

  }
}

void moveTo(int x, int y, float speed) {
  moving = true;

  //calculating motor speeds and direction
  double yawModifier = 2; //this is for the inertial sensor not giving the right numbers to be fed into the system (depending on the position of the inertial sensor on the robot)

  double xValue = cos(yawValue / yawModifier) + (x - xPos);
  double yValue = sin(yawValue / yawModifier) + (y - yPos);


  xValue /= 100;
  yValue /= 100;
    
  //applying those values to the motors
  double frontLeft = (double)((yValue + xValue) * speed);
  double backLeft = (double)((yValue - xValue) * speed);
  double frontRight = (double)((yValue - xValue) * speed);
  double backRight = (double)((yValue + xValue) * speed);


  vertencoder.setPosition(0, degrees);
  strafeencoder.setPosition(0, degrees);

  leftfront.setVelocity(frontLeft, vex::velocityUnits::pct);
  leftback.setVelocity(backLeft, vex::velocityUnits::pct);
  rightfront.setVelocity(frontRight, vex::velocityUnits::pct);
  rightback.setVelocity(backRight, vex::velocityUnits::pct);

  int startPosX = xPos;
  int startPosY = yPos;

  while(!(abs(x-xPos) < 2 && abs(y-yPos) < 2)) {

    yG = abs(y-yPos);
    xG = abs(x-xPos);
    
    leftfront.spin(forward);
    leftback.spin(forward);
    rightfront.spin(forward);
    rightback.spin(forward);

    yPos = strafeencoder.position(degrees) + startPosY;
    xPos = vertencoder.position(degrees) + startPosX;

  }

  yPos = strafeencoder.position(degrees) + startPosY;
  xPos = vertencoder.position(degrees) + startPosX;

  leftfront.stop();
  leftback.stop();
  rightfront.stop();
  rightback.stop();
  moving = false;
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
  thread t(display);

  wait(3, seconds);
  moveTo(0,100,20);
  wait(2, seconds);
  moveTo(100, 100, 20);
}
