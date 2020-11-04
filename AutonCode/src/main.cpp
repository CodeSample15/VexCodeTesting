/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       victoriang                                                */
/*    Created:      Wed Aug 19 2020                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// inertia              inertial      10              
// vertencoder          rotation      11              
// strafeencoder        rotation      12              
// leftfront            motor         1               
// leftback             motor         2               
// rightfront           motor         3               
// rightback            motor         4               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

int currentTask;

void display()
{
  while(true)
  {
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Inertial %f", inertia.rotation(degrees));

    Brain.Screen.setCursor(2, 1);
    Brain.Screen.print("Vert Encoder %f", vertencoder.position(degrees));

    Brain.Screen.setCursor(3, 1);
    Brain.Screen.print("Strafe Encoder %f", strafeencoder.position(degrees));
  }
}

bool isTaskDone(int taskNum) {
  return taskNum == currentTask;
}

void waitTaskFinished(int taskNum) {
  if(!isTaskDone(taskNum)) {
    waitTaskFinished(taskNum);
  }
  else {
    currentTask++;
  }
}

void forwardsdrive(int distancetotravel, int taskNum)
{
  waitTaskFinished(taskNum);
  //distanceleft = distancetotravel; 

  leftfront.setVelocity(40, vex::velocityUnits::pct);
  leftback.setVelocity(40, vex::velocityUnits::pct);
  rightfront.setVelocity(40, vex::velocityUnits::pct);
  rightback.setVelocity(40, vex::velocityUnits::pct);

  vertencoder.setPosition(0,degrees);
  while(vertencoder.position(degrees) < distancetotravel)
  {
    leftfront.spin(forward);
    leftback.spin(forward);
    rightfront.spin(forward);
    rightback.spin(forward);
  }

  leftfront.stop();
  leftback.stop();
  rightfront.stop();
  rightback.stop();

  wait (1,seconds);
}

void backwardsdrive(double distancetotravel, int taskNum)
{
  waitTaskFinished(taskNum);

  leftfront.setVelocity(40, vex::velocityUnits::pct);
  leftback.setVelocity(40, vex::velocityUnits::pct);
  rightfront.setVelocity(40, vex::velocityUnits::pct);
  rightback.setVelocity(40, vex::velocityUnits::pct);
  
  vertencoder.setPosition(0,degrees);
  while(vertencoder.position(degrees)>distancetotravel)
  {
    leftfront.spin(reverse);
    leftback.spin(reverse);
    rightfront.spin(reverse);
    rightback.spin(reverse);
  }
  
  leftfront.stop();
  leftback.stop();
  rightfront.stop();
  rightback.stop();
  wait (1,seconds);
}

void leftstrafe(double distancetotravel, int taskNum)
{
  waitTaskFinished(taskNum);

  strafeencoder.setPosition(0,degrees);
  leftfront.setVelocity(40, vex::velocityUnits::pct);
  leftback.setVelocity(40, vex::velocityUnits::pct);
  rightfront.setVelocity(40, vex::velocityUnits::pct);
  rightback.setVelocity(40, vex::velocityUnits::pct);

  strafeencoder.setPosition(0,degrees);
  while(strafeencoder.position(degrees)<distancetotravel)
  {
    leftfront.spin(reverse);
    leftback.spin(forward);
    rightfront.spin(forward);
    rightback.spin(reverse);
  }

  leftfront.stop();
  leftback.stop();
  rightfront.stop();
  rightback.stop();
  wait (1,seconds);
}

void rightstrafe(double distancetotravel, int taskNum)
{
  waitTaskFinished(taskNum);

  strafeencoder.setPosition(0,degrees);
  leftfront.setVelocity(40, vex::velocityUnits::pct);
  leftback.setVelocity(40, vex::velocityUnits::pct);
  rightfront.setVelocity(40, vex::velocityUnits::pct);
  rightback.setVelocity(40, vex::velocityUnits::pct);
  
  strafeencoder.setPosition(0,degrees);
  while(strafeencoder.position(degrees)>distancetotravel)
  {
    leftfront.spin(forward);
    leftback.spin(reverse);
    rightfront.spin(reverse);
    rightback.spin(forward);
  }
  leftfront.stop();
  leftback.stop();
  rightfront.stop();
  rightback.stop();
  wait (1,seconds);
}

void rightinertialturn(double goaldegrees, int taskNum)
{
  waitTaskFinished(taskNum);

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

  leftfront.stop();
  leftback.stop();
  rightfront.stop();
  rightback.stop();

  wait (1,seconds);
}

void leftinertialturn(double goaldegrees, int taskNum)
{
  waitTaskFinished(taskNum);

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

  leftfront.stop();
  leftback.stop();
  rightfront.stop();
  rightback.stop();

  wait (1,seconds);
}

/*void intake()
{
  rightintake.setVelocity(100, vex::velocityUnits::pct);
  leftintake.setVelocity(100, vex::velocityUnits::pct);

  rightintake.spin(forward);
  leftintake.spin(forward);
}

void outtake(){
  rightintake.setVelocity(50, vex::velocityUnits::pct);
  leftintake.setVelocity(50, vex::velocityUnits::pct);

  leftintake.rotateFor(vex::directionType::fwd,1,vex::rotationUnits::rev,false);
  rightintake.rotateFor(vex::directionType::rev,1,vex::rotationUnits::rev,true);

  rightintake.stop();
  leftintake.stop();

  wait (3,seconds);
}*/

int main() {
  vexcodeInit();

  wait (2, seconds);
  thread t(display);

  currentTask = 0;

  leftstrafe(600, 0);
  rightinertialturn(5, 1);
  forwardsdrive(900, 2);
  backwardsdrive(-50, 3);
  leftinertialturn(-195, 4);
  rightstrafe(-1150, 5);
  rightinertialturn(110, 6);
  forwardsdrive(160, 7);
  leftinertialturn(-100, 8);
}