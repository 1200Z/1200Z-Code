/** @file init.c
 * @brief File for initialization code
 *
 * This file should contain the user initialize() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"
//Functions declared in main.h

/*
 * Runs pre-initialization code. This function will be started in kernel mode one time while the
 * VEX Cortex is starting up. As the scheduler is still paused, most API functions will fail.
 *
 * The purpose of this function is solely to set the default pin modes (pinMode()) and port
 * states (digitalWrite()) of limit switches, push buttons, and solenoids. It can also safely
 * configure a UART port (usartOpen()) but cannot set up an LCD (lcdInit()).
 */
void initializeIO()
{
  //Configure digital inputs(sensors) and outputs(pneumatics)
  pinMode(intake_switch,INPUT);//Digital inputs return TRUE unless pressed
  pinMode(base_bump,INPUT);
  pinMode(brake,OUTPUT);

  //Initial state of pneumatics(not deployed) is 1
  digitalWrite(brake,1);
}

/*
 * Runs user initialization code. This function will be started in its own task with the default
 * priority and stack size once when the robot is starting up. It is possible that the VEXnet
 * communication link may not be fully established at this time, so reading from the VEX
 * Joystick may fail.
 *
 * This function should initialize most sensors (gyro, encoders, ultrasonics), LCDs, global
 * variables, and IMEs.
 *
 * This function must exit relatively promptly, or the operatorControl() and autonomous() tasks
 * will not start. An autonomous mode selection menu like the pre_auton() in other environments
 * can be implemented in this task if desired.
 */
void initialize()
{
  //Initializes the encoders; both encoders increase as the robot drives forward
  rightEncoder = encoderInit(1,2,false);
  leftEncoder = encoderInit(4,3,true);

  //Initializes the gyroscope; values increase going left, decrease going right
  gyro = gyroInit(gyro_port,0);

  //Initializes and clears the LCD screen
  lcdInit(uart1);
  lcdClear(uart1);


  //Autonomous Selection routine
  bool selection = true;

  while(!isEnabled() && selection)
  {
    //Create menu
    lcdSetText(uart1,1," <   Enter   > ");

    //Wait for button press
    while(lcdReadButtons(uart1) == 0) delay(5);

    //Move through the menu
    switch(lcdReadButtons(uart1))
    {
      case 1:
        autonSelect--;
        break;
      case 2:
        selection = false;
        break;
      case 4:
        autonSelect++;
        break;
    }

    //Wait for button release
    while(lcdReadButtons(uart1)) delay(5);

    //Restrict autonSelect variable to number of routines we have
    if(autonSelect < 1) autonSelect = 3;
    else if(autonSelect > 3) autonSelect = 1;

    //Display which autonomous is selected
    switch(autonSelect)
    {
      case 1:
        lcdSetText(uart1,0,"Left 20 Point");
        break;
      case 2:
        lcdSetText(uart1,0,"Prog Skills");
        break;
      case 3:
        lcdSetText(uart1,0,"Right 20 Point");
        break;
    }

    delay(10);
  }
}
