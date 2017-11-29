#include "main.h"

void liftSet(int liftSpeed)
{
  motorSet(4,liftSpeed);
  motorSet(7,-liftSpeed);
}
