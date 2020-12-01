  
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
// leftfront            motor         20              
// leftback             motor         13              
// rightfront           motor         14              
// rightback            motor         18              
// inertia              inertial      10              
// strafeencoder        encoder       E, F            
// vertencoder          encoder       C, D            
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
    Brain.Screen.print("Y Position: %d", yG);

    Brain.Screen.setCursor(5, 1);
    Brain.Screen.print("X Position: %d", xG);

    Brain.Screen.setCursor(6, 1);
    Brain.Screen.print("Yaw Value: %d", yawValue);

    wait(15, msec);
    Brain.Screen.clearScreen();

  }
}

float distanceXY(int x, int y, int x2, int y2) {
  float dx = x - x2;
  float dy = y - y2;

  return sqrt(dx*dx + dy*dy);
}

void moveTo(int x, int y, float speed) {
  moving = true;

  //calculating motor speeds and direction
  double yawModifier = 1; //this is for the inertial sensor not giving the right numbers to be fed into the system (depending on the position of the inertial sensor on the robot)

  vertencoder.setPosition(0, degrees);
  strafeencoder.setPosition(0, degrees);

  int startPosX = xPos;
  int startPosY = yPos;

  float originalSpeed = speed;

  while(!(abs(x-xPos) < 7 && abs(y-yPos) < 7)) {
    double xValue = cos(yawValue / yawModifier) + (x - xPos);
    double yValue = sin(yawValue / yawModifier) + (y - yPos);

    //Normalizing the vector
    float length = sqrt(xValue * xValue + yValue * yValue);

    xValue /= length;
    yValue /= length;
    
    //applying those values to the motors
    double frontLeft = (double)((yValue + xValue));
    double backLeft = (double)((yValue - xValue));
    double frontRight = (double)((yValue - xValue));
    double backRight = (double)((yValue + xValue));

    if(distanceXY(xPos, yPos, x, y) <= 500) {
      speed = originalSpeed / 4;
    }
    else {
      speed = originalSpeed;
    }

    leftfront.setVelocity(frontLeft * speed, vex::velocityUnits::pct);
    leftback.setVelocity(backLeft * speed, vex::velocityUnits::pct);
    rightfront.setVelocity(frontRight * speed, vex::velocityUnits::pct);
    rightback.setVelocity(backRight * speed, vex::velocityUnits::pct);

    yG = yPos;
    xG = xPos;
    
    leftfront.spin(forward);
    leftback.spin(forward);
    rightfront.spin(forward);
    rightback.spin(forward);

    yPos = strafeencoder.position(degrees) - startPosY;
    xPos = vertencoder.position(degrees) + startPosX;
    yawValue = inertia.rotation(degrees);
  }

  xPos = x;
  yPos = -y;

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
  moveTo(0,3100,50);
  wait(2, seconds);
  moveTo(-3000, 3100, 40);
  wait(1, seconds);
  moveTo(-2000, 200, 40);
  wait(1, seconds);
  moveTo(-1000, 1000, 50);
  wait(1, seconds);
  moveTo(0, 0, 30);
}
