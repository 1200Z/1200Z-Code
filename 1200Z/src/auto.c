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

void juicer()
{
  clawDrive(-127, 250);
  robotFunction(Forward, 70, 350);
  clawSet(90);
  delay(500);
  robotFunction(Backward,70,385);
  delay(300);
  clawDrive(-127,400);
}

void stationWagon()
{
  clawSet(30);
  liftDrive(80,950);
  robotFunction(Forward,55,125);
  liftDrive(60,800);
  clawDrive(-127,500);
  robotFunction(Backward,70,50);
}

void autonomous()
{
  stationWagon();
}
