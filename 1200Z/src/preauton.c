#include "main.h"


const int Forward = 1;
const int Backward = 2;
const int LeftPoint = 3;
const int RightPoint = 4;

bool isLessThan(int num1, int num2)
{
  return num1<num2;
}

/*void robotFunction(int chassisDirection, int chassisSpeed, int distance)
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

  while(isLessThan(abs(encoderGet(rightEncoder)),distance))
  {
    chassisSet(leftSpeed, rightSpeed);
  }
  chassisSet(0,0);
}
*/
