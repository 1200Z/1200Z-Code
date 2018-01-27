#include "main.h"
//Functions declared in intake.h

void rollerSet(int speed)
{
  motorSet(int_motor, -speed);
}

void wristSet(int speed)
{
  motorSet(lfb_motor, speed);
  motorSet(rfb_motor, -speed);
}

//Quadratic function that takes the lift pot value and outputs how far back the wrist pot can go
int wristThreshold()
{
  int pot = analogRead(lift_pot);
  return pot*pot/-2342 + 1.1*pot - 50;
}

//Task to control wrist values either by joystick or threshold value
void wristControl(void * parameter)
{
  int error, wristSpeed, y, threshold;
  bool setButton = false;

  while(1)
  {
    threshold = wristThreshold();
    error = threshold - analogRead(wrist_pot);
    y = joystickGetAnalog(2,2);

    if(joystickGetDigital(2,8,JOY_DOWN))
      setButton = true;

    if(joystickGetDigital(2,8,JOY_UP))
      setButton = false;

    if(abs(y) > 5) wristSpeed = -y;

    //Set four-bar to threshold value
    else if(setButton)
    {
      if(abs(error) > 15)
        wristSpeed = error/3;
      else
      {
        wristSpeed = 0;
        setButton = false;
      }
    }
    else wristSpeed = 0;

    wristSet(wristSpeed);

    delay(20);
  }
}
