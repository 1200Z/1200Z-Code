#ifndef _CHASSIS_H_
#define _CHASSIS_H_

void chassisSet(int leftSpeed, int rightSpeed);//Set chassis motors by side
void Brake(int chassisDirection); //Runs motors the opposite direction of the input chassis direction to stop moving
void driveControl(int chassisDirection, int distance, bool doBrake);
void drive(int chassisSpeed, int time);
void driveToGoal(int chassisSpeed);
void gyroTurn(int setpoint);
void driveToStop(int speed);

#endif//_CHASSIS_H_
