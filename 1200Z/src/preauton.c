//this includes "main.h"
#include "main.h"


bool isLessThan(int num1, int num2)
{
  return num1<num2;
}

int sign(int num)
{
  return abs(num)/num;
}

int encoderValue()
{
  return abs(encoderGet(leftEncoder))/2+abs(encoderGet(rightEncoder))/2;
}

void robotFunction(int chassisDirection, int chassisSpeed, int distance)
{
  encoderReset(rightEncoder);
  encoderReset(leftEncoder);
  int leftSpeed, rightSpeed;

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

  while(encoderValue()<distance)
  {
    chassisSet(leftSpeed, rightSpeed);
    delay(1);
  }
  chassisSet(0,0);
  delay(5);
}
