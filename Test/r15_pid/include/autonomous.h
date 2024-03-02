//#include <iostream>
#include "vex.h"
#include "robot-config.h"

using namespace vex;
extern brain Brain;

//------- Aux function definition -------//
void inertial_turn(double desired_angle){
  while(fabs(DrivetrainInertial.heading() - desired_angle) > 2.0){
    Drivetrain.setTurnVelocity(20, percent);
    Drivetrain.turn(left);
  }
}

//--------- Main auton functions ---------//
void auton()
{
  //Drivetrain.setDriveVelocity(80, pct);
  //Complete route of ROBOT 24
  //inertial_turn(45);
  Drivetrain.driveFor(fwd, 100, distanceUnits::cm);
  inertial_turn(45);
  Drivetrain.driveFor(fwd, 10, distanceUnits::cm);
  Drivetrain.driveFor(reverse, 20, distanceUnits::cm);
  inertial_turn(-135);
  Drivetrain.driveFor(reverse, 120, distanceUnits::cm);


}

void skills()
{
}

void move_to_coordinate(double target_x, double target_y, double target_heading)
{

  if (target_x == 0 && target_y != 0)
  {
    Drivetrain.driveFor(fwd, target_y, distanceUnits::cm);
  }
  if (target_y == 0 && target_x != 0)
  {
    double ang = 90;
    if (target_x > 0)
    {
      Drivetrain.turnToHeading(ang, rotationUnits::deg);
    }
    if (target_x < 0)
    {
      Drivetrain.turnToHeading(-ang, rotationUnits::deg);
    }
    Drivetrain.driveFor(fwd, target_x, distanceUnits::cm);
  }
  if (target_x != 0 && target_y != 0)
  {
    double ang = atan(target_y / target_x) * 180 / M_PI;
    double hyp = sqrt(target_x * target_x + target_y * target_y);
    // 1st quadrant
    if (target_x > 0 && target_y > 0)
    {
      Drivetrain.turnToHeading(ang, rotationUnits::deg);
    }
    // 2nd quadrant
    if (target_x < 0 && target_y > 0)
    {
      Drivetrain.turnToHeading(-ang, rotationUnits::deg);
    }
    // 3rd quadrant
    if (target_x < 0 && target_y < 0)
    {
      Drivetrain.turnToHeading(180 - ang, rotationUnits::deg);
    }
    // 4th quadrant
    if (target_x > 0 && target_y < 0)
    {
      Drivetrain.turnToHeading(180 + ang, rotationUnits::deg);
    }
    Drivetrain.driveFor(hyp, distanceUnits::cm);
  }
  DrivetrainInertial.resetHeading();
  if (target_heading != 0)
  {
    Drivetrain.turnToHeading(target_heading, rotationUnits::deg);
  }
  Drivetrain.stop(brakeType::hold);
}