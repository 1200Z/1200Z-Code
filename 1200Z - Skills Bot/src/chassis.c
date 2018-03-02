#include "main.h"
//Functions declared in chassis.h

//Simple function to set the chassis motors
//Two are reversed on each side
void chassisSet(int leftSpeed, int rightSpeed)
{
  motorSet(LF_motor,leftSpeed);//Front
  motorSet(LBA_motor,leftSpeed);
  motorSet(LBB_motor,-leftSpeed);//Back
  motorSet(LBC_motor,leftSpeed);//Front
  motorSet(RF_motor,-rightSpeed);//Front
  motorSet(RBA_motor,-rightSpeed);
  motorSet(RBB_motor,rightSpeed);//Back
  motorSet(RBC_motor,-rightSpeed);//Front
}

//A braking function to halt the robot after driving
//Provides resistance to stop momentum from carrying the robot too far
void Brake(int chassisDirection)
{
  int brakeSpeed = 70;
  switch(chassisDirection)
  {//Set motors in the opposite direction of how they were just moving
    case Forward:
      chassisSet(-brakeSpeed,-brakeSpeed);
      break;
    case Backward:
      chassisSet(brakeSpeed,brakeSpeed);
      break;
    case LeftPoint:
      chassisSet(brakeSpeed,-brakeSpeed);
      break;
    case RightPoint:
      chassisSet(-brakeSpeed,brakeSpeed);
      break;
  }
  delay(40);
  chassisSet(0,0);
}

/*
A more precise chassis movement function for autonomous, which uses
a PD control loop and acceleration functionality. Times out after 250
runs of the loop, which is about 5 seconds.
Additionally, this loop keeps track of any variation between each side of
the drive and adjusts accordingly to ensure straight movement.

Parameters:
ChassisDirection is straightforward, can be forward or backward

distance is the distance in inches we want to travel

doBrake determines if we use the pneumatic brake or the above braking function
after the movement
*/
void driveControl(int chassisDirection, int distance, bool doBrake)
{
  //Declare variables, convert distance to encoder ticks, and reset encoders
  int counter = 0;
  int dist_error, side_error, last_error;
  int max_speed;
  float dist_kP = 0.11;
  float dist_kD = 0.6;
  float side_kP = 0.9;
  int tick_distance = distance*TICKS_PER_INCH;
  int dist_P, dist_D, side_P;
  encoderReset(leftEncoder);
  encoderReset(rightEncoder);
  do {
    //Calculate errors
    int current_enc = encoderValue();
    last_error = dist_error;
    dist_error = tick_distance - current_enc;
    side_error = encoderGet(leftEncoder) - encoderGet(rightEncoder);

    //Set components of speed
    dist_P = dist_kP*dist_error;
    dist_D = dist_kD*(dist_error - last_error);
    side_P = side_kP*side_error;
    if(side_P > 63) side_P = 63;
    if(side_P < -63) side_P = -63;

    int leftSpeed = sign(dist_error)*MIN_DRIVE_POWER + dist_P + dist_D - side_P;
    int rightSpeed = sign(dist_error)*MIN_DRIVE_POWER + dist_P + dist_D + side_P;

    //Cap the motor speed at a value based on how far we have already traveled
    //In order for more precise gradual acceleration
    max_speed = MIN_DRIVE_POWER + abs(current_enc)/3;
    if(leftSpeed > max_speed) leftSpeed = max_speed;
    if(rightSpeed > max_speed) rightSpeed = max_speed;

    //max_speed is always positive, so make it negative when we go backwards
    if(chassisDirection == Backward)
    {
      if(leftSpeed < -max_speed) leftSpeed = -max_speed;
      if(rightSpeed < -max_speed) rightSpeed = -max_speed;
    }

    chassisSet(leftSpeed,rightSpeed);
    counter++;
    delay(20);
  } while(abs(dist_error) > 30 && counter < 250);
  //Use pneumatic brake to stop
  if(doBrake)
  {
    digitalWrite(brake,0);
    delay(200);
    digitalWrite(brake,1);
  }//Otherwise brake with motors
  else Brake(chassisDirection);
  delay(100);
}

//Drives the motors for a time, but monitors side-to-side error
void drive(int chassisSpeed, int time)
{
  int counter = 0;
  int side_P, driveSpeed, max_speed;
  float side_kP = 0.9;
  encoderReset(leftEncoder);
  encoderReset(rightEncoder);
  while(counter < time/20)
  {
    side_P = side_kP*(encoderGet(leftEncoder) - encoderGet(rightEncoder));

    max_speed = MIN_DRIVE_POWER + encoderValue()/2;

    driveSpeed = max_speed < chassisSpeed ? max_speed : chassisSpeed;
    driveSpeed = chassisSpeed;

    if(side_P > 63) side_P = 63;
    if(side_P < -63) side_P = -63;

    chassisSet(driveSpeed - side_P,driveSpeed + side_P);
    delay(20);
    counter++;
  }
  chassisSet(0,0);
}

/*
Drive straight forward until the button on the base intake is bumped
Times out after 4 seconds

Uses similar accelation as above, and keeps track of side-to-side error
*/
void driveToGoal(int chassisSpeed)
{
  int counter = 0;
  int side_P, driveSpeed, max_speed;
  float side_kP = 0.9;
  encoderReset(leftEncoder);
  encoderReset(rightEncoder);
  while(digitalRead(base_bump) && counter < 200)
  {
    if(encoderValue() > 1400)
    {//If no mobile goal is hit, back up and try again
      robotFunction(Backward,60,700,0,0);
      gyroTurn(gyroGet(gyro)+15);
      encoderReset(leftEncoder);
      encoderReset(rightEncoder);
    }
    side_P = side_kP*(encoderGet(leftEncoder) - encoderGet(rightEncoder));

    max_speed = MIN_DRIVE_POWER + encoderValue()/3;

    driveSpeed = max_speed < chassisSpeed ? max_speed : chassisSpeed;
    driveSpeed = chassisSpeed;

    if(side_P > 63) side_P = 63;
    if(side_P < -63) side_P = -63;

    chassisSet(driveSpeed - side_P,driveSpeed + side_P);
    delay(20);
    counter++;
  }
  chassisSet(driveSpeed/2,driveSpeed/2);
  delay(100);
  chassisSet(0,0);
}



/*
Turn to a specified angle using the gyroscope
Runs using a PD control loop for effective deceleration
*/

void gyroTurn(int setpoint)
{
  int error, last_error, max_speed, current_angle, angleTo, angleFrom;
  int counter = 0;
  int leftSpeed, rightSpeed;
  float kP = 0.5;
  float kD = 5.0;
  int init_angle = gyroGet(gyro);
  do {
    current_angle = gyroGet(gyro);
    last_error = error;
    error = setpoint - current_angle;
    leftSpeed = -sign(error)*MIN_DRIVE_POWER - error*kP - last_error*kD;
    rightSpeed = sign(error)*MIN_DRIVE_POWER + error*kP + last_error*kD;

    angleTo = abs(current_angle - init_angle);
    angleFrom = abs(current_angle - setpoint);

    /*
    max_speed is based on how far the robot has turned, or how far it
    has yet to turn, based on whichever is smaller. The first half of a turn
    uses a higher base and quick acceleration, and the last half has
    a smaller ending speed and slower deceleration.
    */
    if(angleTo < angleFrom)
      max_speed = 40 + angleTo/2;
    else
      max_speed = 20 + angleFrom/3;

    if(leftSpeed > max_speed)
      leftSpeed = max_speed;
    else if(leftSpeed < -max_speed)
      leftSpeed = -max_speed;

    if(rightSpeed > max_speed)
      rightSpeed = max_speed;
    else if(rightSpeed < -max_speed)
      rightSpeed = -max_speed;

    counter++;
    chassisSet(leftSpeed, rightSpeed);
    delay(20);
  } while(abs(error) > 4 && counter < 150);

  //Brake the robot by running opposite to the movement that just finished
  chassisSet(-sign(leftSpeed)*50,-sign(rightSpeed)*50);
  delay(40);
  chassisSet(0,0);
  delay(100);
}

//Drives until the encoders stop changing their values
//Uses four variables to keep track of four different encoder values
void driveToStop(int speed)
{
  int left_current,left_past,right_current,right_past;
  do {
    //Set left and right past encoder values
    left_past = left_current;
    right_past = right_current;

    //Drive
    chassisSet(speed,speed);
    delay(150);

    //Set current encoder values
    left_current = encoderGet(leftEncoder);
    right_current = encoderGet(rightEncoder);

    //Break the loop if the encoders have exceed the minimum value (150)
    //and the encoders change by less than 2 counts every 150 milliseconds
  } while((abs(left_current - left_past) > 2 && abs(right_current - right_past) > 2) || abs(left_current) < 150);
  delay(200);
  chassisSet(0,0);
  delay(200);
}
