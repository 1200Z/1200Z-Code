#include "main.h"

void clawSet(int speed)
{
  motorSet(6,speed);
  motorSet(5,speed);
}

void clawDrive(int speed, int time)
{
  clawSet(speed);
  delay(time);
  clawSet(8);
}
