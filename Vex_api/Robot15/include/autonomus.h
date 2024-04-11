#include <iostream>
#include "robot-config.h"
#include "constants.h"
#include "vex_global.h"

using namespace vex;
extern brain Brain;

//--------- Main auton functions ---------//
void auton()
{
  DrivetrainInertial.calibrate();
  Drivetrain.setDriveVelocity(25, pct);

  Drivetrain.driveFor(fwd, 30, distanceUnits::cm);

  Drivetrain.turnToHeading(-30, rotationUnits::deg, 50, rpm, true);

}


//--------- Main skills functions ---------//
void skill()
{

}