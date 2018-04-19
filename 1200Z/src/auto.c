/** @file auto.c
 * @brief File for autonomous code
 *
 * This file should contain the user autonomous() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"
//Functions declared in preauton.h

//Start aligned backwards, with the back of the robot facing two mobile goals
void proSkills()
{
  chassisSet(-127,-127);
  delay(5000);
  chassisSet(0,0);
  delay(2000);
  chassisSet(45,90);
  delay(4000);
  chassisSet(0,0);
}

//Start algined backwards, aiming just in front of opponent colored tile
void leftDefense()
{
  rollerSet(RollerHold);
  chassisSet(-127,-127);
  delay(3000);
  chassisSet(0,0);
}

void rightDefense()
{
  rollerSet(RollerHold);
  chassisSet(-127,-127);
  delay(3500);
  chassisSet(0,0);
  delay(3000);
  chassisSet(-127,-127);
  delay(2000);
  chassisSet(0,0);
}

void rightMogoTen()
{
  liftSet(80);
  delay(400);
  liftSet(0);
  robotFunction(Forward,50,1450,0,0,127,1900);
  delay(100);
  baseSet(-127);
  delay(1100);
  robotFunction(Backward,80,1350,0,0,-127,400);
  delay(200);
  robotFunction(RightPoint,70,450,0,0,0,0);
  robotFunction(0,0,0,0,0,127,1600);
  delay(200);
  robotFunction(Backward,127,150,0,0,0,0);
}

void leftMogoTen()
{
  liftSet(80);
  delay(400);
  liftSet(0);
  robotFunction(Forward,50,1450,0,0,127,1900);
  delay(100);
  baseSet(-127);
  delay(1100);
  robotFunction(Backward,80,1350,0,0,-127,400);
  delay(200);
  robotFunction(LeftPoint,70,450,0,0,0,0);
  robotFunction(0,0,0,0,0,127,1600);
  delay(200);
  robotFunction(Backward,127,150,0,0,0,0);
}

void leftMogoFive()
{
  liftSet(80);
  delay(400);
  liftSet(0);
  robotFunction(Forward,50,1450,0,0,127,1900);
  delay(100);
  baseSet(-127);
  delay(1100);
  robotFunction(Backward,80,1000,0,0,-127,400);
  delay(200);
  robotFunction(LeftPoint,70,450,0,0,0,0);
  robotFunction(0,0,0,0,0,127,1600);
  delay(200);
  robotFunction(Backward,127,150,0,0,0,0);
}

void rightMogoFive()
{
  liftSet(80);
  delay(400);
  liftSet(0);
  robotFunction(Forward,50,1450,0,0,127,1900);
  delay(100);
  baseSet(-127);
  delay(1100);
  robotFunction(Backward,80,1000,0,0,-127,400);
  delay(200);
  robotFunction(RightPoint,70,450,0,0,0,0);
  robotFunction(0,0,0,0,0,127,1600);
  delay(200);
  robotFunction(Backward,127,150,0,0,0,0);
}
/*
* Runs the user autonomous code. This function will be started in its own task with the default
* priority and stack size whenever the robot is enabled via the Field Management System or the
* VEX Competition Switch in the autonomous mode. If the robot is disabled or communications is
* lost, the autonomous task will be stopped by the kernel. Re-enabling the robot will restart
* the task, not re-start it from where it left off.
*
* Code running in the autonomous task cannot access information from the VEX Joystick. However,
* the autonomous function can be invoked from another task if a VEX Competition Switch is not
* available, and it can access joystick information if called in this way.
*
* The autonomous task may exit, unlike operatorControl() which should never exit. If it does
* so, the robot will await a switch to another mode or disable/enable cycle.
*/
void autonomous()
{
    if(analogRead(auton_select) <= 800)
      leftMogoTen();
    else if(analogRead(auton_select) > 800 && analogRead(auton_select) <= 1600)
      leftMogoFive();
    else if(analogRead(auton_select) > 1600 && analogRead(auton_select) <= 2400)
      leftDefense();
    else if(analogRead(auton_select) > 2400 && analogRead(auton_select) <= 3200)
      rightMogoFive();
    else if(analogRead(auton_select) > 3200)
      rightMogoTen();
}
