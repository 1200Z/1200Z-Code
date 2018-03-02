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

//Score one mobile goal in the twenty-point zone, starting from the left side
void TwentyLeft()
{
  //Global variable for intake direction, used in baseMove task
  //Begin baseMove task, putting the intake down to the ground
  baseDirection = Down;
  TaskHandle base1 = taskCreate(baseMove,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);

  //Drive to and intake mobile goal
  driveToGoal(70);
  baseSet(-90);
  delay(1000);
  baseSet(0);

  //Correct alignment
  gyroTurn(0);

  //Back up, turn, and move in front of the starting bar
  robotFunction(Backward,70,900,0,0);
  gyroTurn(-125);
  driveControl(Forward,24,false);

  //Turn to face the twenty point zone and lower the mobile goal into scoring height
  gyroTurn(-220);
  baseSetHeight = 1000;
  TaskHandle base2 = taskCreate(baseControl,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);

  //Score mobile goal
  drive(75,560);
  delay(500);
  robotFunction(Backward,127,150,0,0);
  taskDelete(base2);

  //Bring mobile goal intake in and drive towards opponent colored tile
  baseSet(-50);
  gyroTurn(-155);
  robotFunction(Backward,100,300,0,0);
  gyroTurn(-190);
  driveToStop(-127);
}

void TwentyRight()
{
  //Global variable for intake direction, used in baseMove task
  //Begin baseMove task, putting the intake down to the ground
  baseDirection = Down;
  TaskHandle base1 = taskCreate(baseMove,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);

  //Drive to and intake mobile goal
  driveToGoal(70);
  baseSet(-90);
  delay(1000);
  baseSet(0);

  //Correct alignment
  gyroTurn(0);

  //Back up, turn, and move in front of the starting bar
  robotFunction(Backward,70,900,0,0);
  gyroTurn(125);
  driveControl(Forward,24,false);

  //Turn to face the twenty point zone and lower the mobile goal into scoring height
  gyroTurn(220);
  baseSetHeight = 1000;
  TaskHandle base2 = taskCreate(baseControl,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);

  //Score mobile goal
  drive(75,560);
  delay(500);
  robotFunction(Backward,127,150,0,0);
  taskDelete(base2);

  //Bring mobile goal intake in and drive towards opponent colored tile
  baseSet(-50);
  gyroTurn(155);
  robotFunction(Backward,100,300,0,0);
  gyroTurn(190);
  driveToStop(-127);
}
//Start aligned backwards, with the back of the robot facing two mobile goals

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
  switch(autonSelect)
  {
    case 1:
      TwentyLeft();
      break;
    case 2:
      ProSkills1();
      ProSkills2();
      ProSkills3();
      break;
    case 3:
      TwentyRight();
      break;
  }
}
