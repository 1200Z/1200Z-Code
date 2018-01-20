#include "main.h"
//Functions declared in intake.h

void rollerSet(int speed)
{
  motorSet(int_motor, speed);
}

void wristSet(int speed)
{
  motorSet(lfb_motor, speed);
  motorSet(rfb_motor, -speed);
}
