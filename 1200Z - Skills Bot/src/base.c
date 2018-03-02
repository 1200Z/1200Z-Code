#include "main.h"
//Functions declared in base.h


//Simple function to set base intake motors, the right side is reversed
void baseSet(int speed)
{
  motorSet(LMG_motor,speed);
  motorSet(RMG_motor,-speed);
}

//Drive the base intake for a set time, in milliseconds
void baseDrive(int speed, int time)
{
  baseSet(speed);
  delay(time);
  baseSet(0);
}

//Task to run the mobile goal intake all the way down or all the way up
void baseMove(void * parameter)
{
  bool keepGoing = true;
  int speed;
  while(keepGoing)
  {
    switch(baseDirection)
    {//If up, read potentiometer values, if down, read limit switch value
    //Then set speed according to the desired direction
      case Up:
        speed = -127;
        keepGoing = analogRead(intake_pot) - 12;
        break;
      case Down:
        speed = 127;
        keepGoing = digitalRead(intake_switch);
        break;
      default:
        speed = 0;
    }
    baseSet(speed);
    delay(20);
  }
  delay(100);

  //Hold speed
  baseSet(sign(speed)*6);
}


//Task to maintain the base intake at a certain potentiometer value
//Runs while holding a mobile goal
void baseControl(void * parameter)
{
  //Proportional constant for error to motor speed
  float kP = 0.3;
  int error, speed;
  while(1)
  {
    //Read error
    error = baseSetHeight - analogRead(intake_pot);

    //Exception in case we want this task to not run for a bit
    if(baseSetHeight == 0) baseSet(0);

    //Set motor speed to a minimum value plus proportional value based on error
    else if(abs(error) > 50)
    {
      speed = sign(error)*10 + error*kP;
      baseSet(speed);
    }
    else baseSet(0);
    delay(20);
  }
}

//Same function as above, except for use in driver control rather than autonomous
void driverBaseControl(void * parameter)
{
  float kP = 0.3;
  int error, speed;
  while(abs(joystickGetAnalog(1,3)) < 5 && holdingBase)
  {
    error = baseSetHeight - analogRead(intake_pot);

    if(baseSetHeight == 0) baseSet(0);
    else if(abs(error) > 50)
    {
      speed = sign(error)*10 + error*kP;
      baseSet(speed);
    }
    else baseSet(0);
    delay(20);
  }
}
