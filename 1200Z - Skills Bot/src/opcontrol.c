/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
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
 * Runs the user operator control code. This function will be started in its own task with the
 * default priority and stack size whenever the robot is enabled via the Field Management System
 * or the VEX Competition Switch in the operator control mode. If the robot is disabled or
 * communications is lost, the operator control task will be stopped by the kernel. Re-enabling
 * the robot will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the VEX Cortex will
 * run the operator control task. Be warned that this will also occur if the VEX Cortex is
 * tethered directly to a computer via the USB A to A cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is available and
 * the scheduler is operational. However, proper use of delay() or taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */
void driverControl()
{
	//Variable declarations for joystick controls
	int x,y,z;
	int leftSpeed, rightSpeed;//, wristSpeed;
	taskCreate(driverBaseControl,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
	holdingBase = false;
	bool slowDown = false;
	while (1)
	{
		x = joystickGetAnalog(1,1);//Right x-axis
		y = joystickGetAnalog(1,2);//Right y-axis
		z = joystickGetAnalog(1,3);//Left y-axis

		//Set speeds based on joystick values
		leftSpeed = y+x;
		rightSpeed = y-x;



		//Enables task for controlling base intake
		//Each of these if statements uses a while loop to delay until button release
		if(joystickGetDigital(1,8,JOY_UP))
		{
			holdingBase = true;
			while(joystickGetDigital(1,8,JOY_UP)) delay(5);
		}

		//Sets base intake control task to the 10-point scoring height
		if(joystickGetDigital(1,8,JOY_LEFT))
		{
			baseSetHeight = 1300;
			while(joystickGetDigital(1,8,JOY_LEFT)) delay(5);
		}

		//Sets base intake control task to the 20-point scoring height
		if(joystickGetDigital(1,8,JOY_RIGHT))
		{
			baseSetHeight = 1000;
			while(joystickGetDigital(1,8,JOY_RIGHT)) delay(5);
		}

		//Disables base intake control task
		if(joystickGetDigital(1,8,JOY_DOWN))
		{
			holdingBase = false;
			while(joystickGetDigital(1,8,JOY_DOWN)) delay(5);
		}

		//Toggles pneumatic brakes by writing the opposite of the current value
		if(joystickGetDigital(1,6,JOY_UP))
		{
			digitalWrite(brake,!digitalRead(brake));
			while(joystickGetDigital(1,6,JOY_UP)) delay(5);
		}

		//Puts the drive in slow mode, in which all motor speed are halved
		if(joystickGetDigital(1,7,JOY_DOWN))
		{
			slowDown = !slowDown;
			while(joystickGetDigital(1,7,JOY_DOWN)) delay(5);
		}

		//Execution of "slow mode"
		if(slowDown)
		{
			leftSpeed = leftSpeed/2;
			rightSpeed = rightSpeed/2;
		}

		//Set motors
		chassisSet(leftSpeed, rightSpeed);

		if(abs(z)>5) baseSet(z);
		else(baseSet(0));
	}
}

void operatorControl()
{
	driverControl();
}
