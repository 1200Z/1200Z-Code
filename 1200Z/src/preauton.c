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
  return (analogRead(left_pot)+analogRead(right_pot))/2;
}

void robotFunction(int chassisDirection, int chassisSpeed, int distance, int liftSpeed, int height)
{
  encoderReset(rightEncoder);
  encoderReset(leftEncoder);
  int leftSpeed, rightSpeed;
  bool keepGoing = true;
  bool keepChassisGoing, keepLiftGoing;

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
      leftSpeed = chassisSpeed;
      rightSpeed = -chassisSpeed;
      break;
    case RightPoint:
      leftSpeed = -chassisSpeed;
      rightSpeed = chassisSpeed;
      break;
    default:
      leftSpeed = 0;
      rightSpeed = 0;
  }

  while(keepGoing)
  {
    keepChassisGoing = encoderValue()<distance;

    if(height != 0)
    {
      keepLiftGoing = abs(height - potValue()) > 7;
    }
    else keepLiftGoing = false;

    if(keepChassisGoing)
    {
      chassisSet(leftSpeed, rightSpeed);
    }

    if(keepLiftGoing)
    {
      liftSet(sign(height - potValue())*liftSpeed);
    }

    keepGoing = keepChassisGoing || keepLiftGoing;
    delay(20);
  }
  chassisSet(0,0);
}
