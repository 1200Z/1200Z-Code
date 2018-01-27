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
	int x,y,z,w,v;
	int leftSpeed, rightSpeed, rollerSpeed, wristSpeed;
	while (1)
	{
		x = joystickGetAnalog(1,1);//Right x-axis
		y = joystickGetAnalog(1,2);//Right y-axis
		z = joystickGetAnalog(2,3);//Partner left y-axis
		w = joystickGetAnalog(2,2);//Partner right y-axis
		v = joystickGetAnalog(1,3);//Left y-axis

		leftSpeed = y+x;
		rightSpeed = y-x;

		if(sign(leftSpeed) != sign(rightSpeed))
		{
			leftSpeed = leftSpeed*0.7;
			rightSpeed = rightSpeed*0.7;
		}

		if(joystickGetDigital(2,6,JOY_UP))
		{
			rollerSpeed = -127;
		}
		else if(joystickGetDigital(2,6,JOY_DOWN))
		{
			rollerSpeed = 127;
		}
		else
		{
			rollerSpeed = RollerHold;
		}

		if(abs(w) > 5)
		{
			wristSpeed = -w;
		}
		else wristSpeed = WristHold;

		if(analogRead(wrist_pot) < wristThreshold() &&  wristSpeed < 0)
		{
			wristSpeed = 0;
		}

		chassisSet(leftSpeed, rightSpeed);
		rollerSet(rollerSpeed);
		baseSet(v);
		//wristSet(wristSpeed);

		if(abs(z) > 5) liftSet(z);
		else liftSet(LiftHold);
	}
}

void operatorControl()
{
	taskCreate(wristControl,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
	driverControl();
	/*int liftPot, wristPot, wrist;

	while(1)
	{
		liftPot = analogRead(lift_pot);
		wrist = analogRead(wrist_pot);
		wristPot = wristThreshold();
		printf("%d ",liftPot);
		printf("%d ", wrist);
		printf("%d\n", wristPot);
		delay(20);
	} */
}
