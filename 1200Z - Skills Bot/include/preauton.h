#ifndef _PREAUTON_H_
#define _PREAUTON_H_

int sign(int num);//Returns the sign(1 or -1) of an integer
int encoderValue();//Returns the average absolute value of the chassis encoders
int potValue();//Returns the average value of the lift potentiometers


/*Function for majority of autonomous movements, controls the chassis with
* 3 parameters - direction of movement, speed of motors, and target encoder value
* Lift is controlled with two - lift speed and target potentiometer value */
void robotFunction(int chassisDirection, int chassisSpeed, int distance, int baseSpeed, long baseTime);
void straightScore(int distance);
void ProSkills1();
void ProSkills2();
void ProSkills3();

#endif//_PREAUTON_H_
