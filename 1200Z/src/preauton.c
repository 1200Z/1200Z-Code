#include "main.h"
//Functions declared in preauton.h

int sign(int num)
{
  return abs(num)/num;
}

int encoderValue()
{
  return abs(encoderGet(leftEncoder))/2+abs(encoderGet(rightEncoder))/2;
}

int potValue()
{
  //return (analogRead(left_pot)+analogRead(right_pot))/2;
  return analogRead(lift_pot);
}


void robotFunction(int chassisDirection, int chassisSpeed, int distance, int liftSpeed, int height, int baseSpeed, long baseTime)
{
  encoderReset(rightEncoder);
  encoderReset(leftEncoder);
  int leftSpeed, rightSpeed;
  bool keepGoing = true;
  bool keepChassisGoing, keepLiftGoing, keepBaseGoing;
  long init_time = micros();

  switch(chassisDirection)
  {
    case Forward:
      leftSpeed = chassisSpeed;
      rightSpeed = chassisSpeed;
      break;
    case Backward:
      leftSpeed = -chassisSpeed;
      rightSpeed = -chassisSpeed;
      break;
    case LeftPoint:
      leftSpeed = -chassisSpeed;
      rightSpeed = chassisSpeed;
      break;
    case RightPoint:
      leftSpeed = chassisSpeed;
      rightSpeed = -chassisSpeed;
      break;
    default:
      leftSpeed = 0;
      rightSpeed = 0;
  }



  while(keepGoing)
  {
    int delta_time = micros() - init_time;
    keepChassisGoing = encoderValue()<distance;

    if(height != 0)
    {
      keepLiftGoing = abs(height - potValue()) > 7;
    }
    else keepLiftGoing = false;

    keepBaseGoing = baseTime*1000 >= delta_time;

    if(keepChassisGoing)
      chassisSet(leftSpeed, rightSpeed);
    else chassisSet(0,0);

    if(keepLiftGoing)
      liftSet(sign(height - potValue())*liftSpeed);
    else liftSet(0);

    if(keepBaseGoing)
      baseSet(baseSpeed);
    else baseSet(0);

    keepGoing = keepChassisGoing || keepLiftGoing || keepBaseGoing;
    delay(20);
  }
  chassisSet(0,0);
  baseSet(0);
  liftSet(0);
}
