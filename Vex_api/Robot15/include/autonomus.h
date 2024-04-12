#include <iostream>
#include "robot-config.h"
//#include "constants.h"
#include "vex_global.h"

using namespace vex;
//extern brain Brain;

//--------- Main auton functions ---------//
void auton()
{
  DrivetrainInertial.calibrate();
  Drivetrain.setDriveVelocity(25, pct);

  Drivetrain.driveFor(fwd, 120, distanceUnits::cm);

}


//--------- Main skills functions ---------//
void skill()
{

}