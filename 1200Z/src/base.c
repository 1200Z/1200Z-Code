#include "main.h"
//Functions declared in base.h

void baseSet(int speed)
{
  motorSet(base_motor,speed);
}

void baseDrive(int speed, int time)
{
  baseSet(speed);
  delay(time);
  baseSet(0);
}
