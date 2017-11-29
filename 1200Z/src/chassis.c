#include "main.h"


void chassisSet(int leftSpeed, int rightSpeed)
{
  motorSet(8,-rightSpeed);//Front
  motorSet(9,-leftSpeed);//Middle
  motorSet(10,leftSpeed);//Back
  motorSet(1,-rightSpeed);//Back
  motorSet(2,rightSpeed);//Middle
  motorSet(3,leftSpeed);//Front
}
