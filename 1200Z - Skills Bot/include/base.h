#ifndef _BASE_H_
#define _BASE_H_

void baseSet(int speed);//Set base motor
void baseDrive(int speed, int time);//Drive base motor for a certain time
void baseMove(void * parameter);
void baseControl(void * parameter);
void driverBaseControl(void * parameter);

#endif//_BASE_H_
