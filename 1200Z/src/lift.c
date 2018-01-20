#include "main.h"
//Functions declared in lift.h

void liftSet(int liftSpeed)
{
  motorSet(ll_motor,liftSpeed);
  motorSet(rl_motor,-liftSpeed);//Right lift motor reversed
}

void liftDrive(int liftSpeed, int height)
{
  //initialize variables for tracking position
  int difference = height - potValue();
  int timeOutCounter;

  //While we are outside of 7 pot ticks of our target
  while(abs(difference) > 7)
  {
    //Move up if height is greater, down if potValue() is greater
    liftSet(sign(difference)*liftSpeed);
    wait(5);
    difference = height - potValue();
    timeOutCounter++;
  }
  liftSet(0);//Stop the lift
}
