#include "main.h"
//Functions declared in chassis.h

void chassisSet(int leftSpeed, int rightSpeed)
{
  motorSet(fld_motor,-leftSpeed);//Front
  motorSet(bld_motor,leftSpeed);//Back
  motorSet(frd_motor,-rightSpeed);//Front
  motorSet(brd_motor,rightSpeed);//Back
}
