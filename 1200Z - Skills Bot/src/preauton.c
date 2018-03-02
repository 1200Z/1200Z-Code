#include "main.h"
//Functions declared in preauton.h

//Gives the sign of an integer. Returns -1, 0, or 1
int sign(int num)
{
  return abs(num)/num;
}

//Returns the average value of the left and right encoders
int encoderValue()
{
  return abs(encoderGet(leftEncoder))/2+abs(encoderGet(rightEncoder))/2;
}

/*
Autonomous movement function without acceleration. Prioritizes speed over
accuracy, and can run multiple subsystems simultaneously

Parameters:
chassisDirection: Forward, Backward, LeftPoint(turn), rightPoint
chassisSpeed: desired motor speed for chassis(should always input positive)
distance: encoder ticks to move chassis
baseSpeed: desired motor speed for base intake
baseTime: amount of time in milliseconds to run the base intake
*/
void robotFunction(int chassisDirection, int chassisSpeed, int distance, int baseSpeed, long baseTime)
{
  encoderReset(rightEncoder);
  encoderReset(leftEncoder);
  int leftSpeed, rightSpeed;
  bool keepGoing = true;
  bool keepChassisGoing = true;
  bool keepBaseGoing = true;
  long init_time = micros();

  switch(chassisDirection)
  {//Determines motor speeds and direction for each side of the chassis
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
    //Calculate elapsed time
    int delta_time = micros() - init_time;

    //Determine if the chassis needs to continue moving
    if(keepChassisGoing)
      keepChassisGoing = encoderValue()<distance;

    //Determine if the base intake needs to continue moving
    if(keepBaseGoing)
      keepBaseGoing = baseTime*1000 >= delta_time;

    //Sets chassis if it needs to move
    if(keepChassisGoing)
      chassisSet(leftSpeed, rightSpeed);
    else chassisSet(0,0);

    //Sets base intake if it needs to move
    if(keepBaseGoing)
      baseSet(baseSpeed);
    else baseSet(0);

    //Keep running the loop if either subsystem needs to keep moving
    keepGoing = keepChassisGoing|| keepBaseGoing;
    delay(20);
  }
  //Stop all motors
  Brake(chassisDirection);
  baseSet(0);
}

//Score a mobile goal in the ten-point zone, starting facing it
void straightScore(int distance)
{
  //Drive to the bar
  driveControl(Forward,distance,false);
  chassisSet(40,40);
  delay(400);

  //Lower base intake to release mobile goal
  baseSetHeight = 1300;
}

/*
Programming Skills routine, divided into three parts to make it easier to test
ProSkills1: Score mobile goal in twenty-point zone
ProSkills2: Score two mobile goals in ten-point zone
ProSkills3: Score one mobile goal in opposite twenty-point zone and one the ten
*/

//Start about 5 inches from the side of the field, facing the mobile goal
void ProSkills1()
{
  //Begin baseMove task, putting the intake down to the ground
  baseDirection = Down;
  TaskHandle base1 = taskCreate(baseMove,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);

  //Drive and pick up mobile goal in colored square
  driveToGoal(70);
  baseSet(-90);
  delay(1000);

  //Re-align and back up
  gyroTurn(0);
  robotFunction(Backward,70,900,0,0);

  //Turn and drive in front of the starting bar
  gyroTurn(-125);
  driveControl(Forward,24,false);
  gyroTurn(-220);

  //Lower mobile goal to scoring height
  baseSetHeight = 1000;
  TaskHandle base2 = taskCreate(baseControl,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);

  //Score mobile goal and back up
  drive(75,560);
  delay(500);
  robotFunction(Backward,127,150,0,0);
  taskDelete(base2);

  //Bring in mobile goal intake and move to wall for alignment
  baseSet(-50);
  gyroTurn(-155);
  robotFunction(Backward,60,22*TICKS_PER_INCH,0,0);
  gyroTurn(-90);
  driveToStop(-30);
  baseSet(0);
}

//Start against the wall, facing in towards the center of the field
void ProSkills2()
{
  //Reset gyro
  gyroReset(gyro);

  //Bring base intake to the ground
  baseDirection = Down;
  taskCreate(baseMove,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
  delay(500);

  //Drive and pick up mobile goal
  driveToGoal(70);
  baseSet(-100);
  delay(1000);
  baseSet(0);

  //Turn and score mobile goal in 10-pt
  gyroTurn(-120);
  encoderReset(leftEncoder);
  encoderReset(rightEncoder);
  baseSetHeight = 1100;
  TaskHandle base3 = taskCreate(baseControl,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
  straightScore(27);

  //Back up and turn to face next goal
  robotFunction(Backward,45,encoderValue(),0,0);
  taskDelete(base3);
  delay(500);
  gyroTurn(-58);

  //Move intake down and drive forward, intaking another mobile goal
  taskCreate(baseMove,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
  encoderReset(leftEncoder);
  encoderReset(rightEncoder);
  driveToGoal(80);
  //890 is the toal distance we want for this movement, including the distance
  //we travel to pick up the mobile goal
  robotFunction(Forward,80,890-encoderValue(),0,0);

  //Intake mobile goal
  baseSet(-95);
  delay(1000);
  baseSet(0);

  //Turn and score mobile goal
  gyroTurn(-136);
  driveControl(Forward,33,false);
  baseSetHeight = 1100;
  TaskHandle base4 = taskCreate(baseControl,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
  gyroTurn(-110);
  drive(70,500);
  baseSetHeight = 1350;
  delay(600);
  robotFunction(Backward,50,50,0,0);
  delay(100);
  taskDelete(base4);

  //Move back to wall where this function began to re-align
  baseSet(-127);
  delay(300);
  gyroTurn(-38);
  baseSet(0);
  robotFunction(Backward,60,28*TICKS_PER_INCH,0,0);
  gyroTurn(0);
  driveToStop(-45);
  baseSet(0);
}

//Begin with the same orientaiton and ProSkills2
void ProSkills3()
{
  //Reset gyro
  gyroReset(gyro);

  //Move base intake to the ground
  baseDirection = Down;
  taskCreate(baseMove,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
  delay(300);

  //Drive forward and sweep aways cones on the left
  driveControl(Forward,27,false);
  gyroTurn(80);
  delay(100);
  gyroTurn(49);

  //Drive towards a mobile goal and pick it up
  driveToGoal(80);
  baseSet(-95);
  delay(1000);
  baseSet(0);

  //Maneuver to the front of the starting bar opposite of where we started
  driveControl(Forward,22,false);
  gyroTurn(-10);
  driveControl(Forward,13,false);
  gyroTurn(49);

  //Lower base intake and score mobile goal
  baseSetHeight = 1000;
  TaskHandle base5 = taskCreate(baseControl,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
  drive(70,530);
  delay(500);

  //Get in position for final mobile goal
  robotFunction(Backward,127,150,0,0);
  taskDelete(base5);
  baseSet(-50);
  gyroTurn(-40);
  driveControl(Forward,13,false);
  gyroTurn(-117);

  //Bring down base intake and drive towards mobile goal
  taskCreate(baseMove,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
  delay(800);
  driveToGoal(80);

  //Life it off the ground and gun it for the 10-pt zone across from where we just scored
  baseSetHeight = 1100;
  TaskHandle base6 = taskCreate(baseControl,TASK_DEFAULT_STACK_SIZE,NULL,TASK_PRIORITY_DEFAULT);
  delay(300);
  driveControl(Forward,64,false);

  //Score mobile goal
  baseSetHeight = 1350;
  delay(300);
  robotFunction(Backward,80,150,0,0);

  //Turn and park
  gyroTurn(gyroGet(gyro)-40);
  driveToStop(-100);
}
