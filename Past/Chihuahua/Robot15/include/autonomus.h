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

  //Primera ida
  Drivetrain.driveFor(fwd, 70, distanceUnits::cm);
  
  /*-----------------------------
  * Vuelta para empujar el trible
  * -----------------------------*/
  Drivetrain.turnToHeading(-30, rotationUnits::deg, 50, rpm, true);
  IndexerRight.set(true);
  Drivetrain.turnToHeading(185, rotationUnits::deg, 80, rpm, true);
  IndexerRight.set(false);

  /*----------------------------------
  * Se acomoda para recoger el trible
  * ----------------------------------*/
  Drivetrain.driveFor(reverse, 10, distanceUnits::cm);
  Drivetrain.turnToHeading(135, rotationUnits::deg, 30, rpm, true);
  Drivetrain.driveFor(reverse, 26, distanceUnits::cm);//25
  Drivetrain.turnToHeading(225, rotationUnits::deg, 30, rpm, true);
  Drivetrain.driveFor(reverse, 20, distanceUnits::cm);

  /*----------------
  * Recoge el trible
  * ----------------*/
  Climber.spin(reverse, 45, percent);
  wait(5.8, seconds);
  Climber.stop();

  /*---------------
  * Saca el trible
  * ---------------*/
  Drivetrain.setDriveVelocity(35, pct);
  Drivetrain.driveFor(fwd, 90, distanceUnits::cm, true);
  Drivetrain.setDriveVelocity(25, pct);
  
  /*---------------------------------
  * Se acomoda para soltar el trible
  * ---------------------------------*/
  Drivetrain.turnToHeading(180, rotationUnits::deg, 30, rpm, true);

  /*-----------------
  * Suelta el trible
  * -----------------*/
  Climber.spin(fwd, 45, percent);
  wait(2, seconds);
  Climber.stop();

  /*-----------------
  * Empuja el trible
  * -----------------*/
  Drivetrain.setDriveVelocity(70, pct);
  Drivetrain.driveFor(reverse, 50, distanceUnits::cm);
  Drivetrain.driveFor(fwd, 10, distanceUnits::cm);
  Drivetrain.driveFor(reverse, 20, distanceUnits::cm);

  /*-----------------------------------
  * Se acomoda para tocar el tubo rojo
  * -----------------------------------*/
  Drivetrain.setDriveVelocity(45, pct);
  Drivetrain.driveFor(fwd, 45, distanceUnits::cm);
  Drivetrain.turnToHeading(270, rotationUnits::deg, 30, rpm, true);
  Drivetrain.driveFor(reverse, 50, distanceUnits::cm);
  Drivetrain.turnToHeading(345, rotationUnits::deg, 30, rpm, true);
}



void skill()
{
  DrivetrainInertial.calibrate();
  while (DrivetrainInertial.isCalibrating()) {
    wait(25, msec);
  }
  Drivetrain.setDriveVelocity(28, pct);
  
  /*------------------
  * salida del pasillo
  * ------------------*/
  Drivetrain.driveFor(fwd, 98, distanceUnits::cm);
  Drivetrain.turnToHeading(90, rotationUnits::deg, 100, rpm, true);

  /*-----------------------------------------
  * Se acomoda y llega a la altura de la red
  * -----------------------------------------*/
  Drivetrain.driveFor(fwd, 45, distanceUnits::cm);
  Drivetrain.turnToHeading(0, rotationUnits::deg, 100, rpm, true);
  Drivetrain.driveFor(reverse, 10, distanceUnits::cm);
  Drivetrain.turnToHeading(90, rotationUnits::deg, 100, rpm, true);

  /*-------------------------------------------------------
  * Abre las alitas para actuar como pared con los triballs
  * -------------------------------------------------------*/
  IndexerLeft.set(true);
  IndexerRight.set(true);
  wait(10, seconds);
  IndexerLeft.set(false);
  IndexerRight.set(false);

  /*-------------------------------------------------------------------------
  * Llega al tubo negro para acomodarse y posteriormente empujar los triballs
  * -------------------------------------------------------------------------*/
  Drivetrain.turnToHeading(180, rotationUnits::deg, 100, rpm, true);
  Drivetrain.driveFor(fwd, 45, distanceUnits::cm);
  Drivetrain.turnToHeading(90, rotationUnits::deg, 100, rpm, true);

  //int i;
  for (int i=0;i<5;i++){
    // vuelta a red
    Drivetrain.driveFor(fwd, 50, distanceUnits::cm);//50*
    Drivetrain.turnToHeading(0, rotationUnits::deg, 100, rpm, true);
    IndexerLeft.set(true);
    IndexerRight.set(true);

    // golpe a la red
    Drivetrain.setDriveVelocity(60, pct);
    Drivetrain.driveFor(fwd, 50, distanceUnits::cm);
    Drivetrain.setDriveVelocity(28, pct);

    // regreso al tubo
    // IndexerLeft.set(false);
    // IndexerRight.set(false);
    Drivetrain.driveFor(reverse, 50, distanceUnits::cm);
    //IndexerLeft.set(false);
    IndexerRight.set(false);
    Drivetrain.turnToHeading(90, rotationUnits::deg, 100, rpm, true);
    Drivetrain.driveFor(reverse, 50, distanceUnits::cm);

    //esperar triballs
    wait(8, seconds);
  }

}