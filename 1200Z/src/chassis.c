#include "main.h"

void chassisSet(int leftSpeed, int rightSpeed)
{
  motorSet(9,leftSpeed);
  motorSet(10,leftSpeed);
  motorSet(1,rightSpeed);
  motorSet(2,rightSpeed);
}
