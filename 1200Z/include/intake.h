#ifndef _INTAKE_H_
#define _INTAKE_H_

void rollerSet(int speed);//Set motor on the Goliath intake
void wristSet(int speed);//Set the motors on the four-bar
int wristThreshold();//Determines wrist pot value by lift height
void wristControl(void *parameter);//Secondary task to control four-bar

#endif//INTAKE_H_
