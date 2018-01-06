#include "main.h"

void liftSet(int liftSpeed)
{
  motorSet(4,liftSpeed);
  motorSet(7,-liftSpeed);
}

void liftDrive(int liftSpeed, int height)
{
  int potValue = (analogRead(left_pot) + analogRead(right_pot))/2;
  int difference = height - potValue;
  int timeOutCounter;
  while(abs(difference) > 7)
  {
    liftSet(sign(difference)*liftSpeed);
    wait(5);
    potValue = (analogRead(left_pot) + analogRead(right_pot))/2;
    difference = height - potValue;
    timeOutCounter++;
  }
  liftSet(0);
}
