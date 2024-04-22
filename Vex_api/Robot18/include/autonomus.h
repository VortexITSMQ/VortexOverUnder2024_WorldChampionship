
#include "robot-config.h"
#include "constants.h"
//#include "vex_global.h"

using namespace vex;
extern brain Brain;

void girarDuracion(int duracion, double velocidad, bool turn_right) {
    // Configurar la velocidad de giro de los motores
    Drivetrain.setTurnVelocity(velocidad, pct);
    // Girar el robot durante la duración especificada
    (turn_right) ? Drivetrain.turn(vex::turnType::right, velocidad, vex::velocityUnits::pct) : Drivetrain.turn(vex::turnType::left, velocidad, vex::velocityUnits::pct);
    // o vex::turnType::right
    wait(duracion, msec);
    Drivetrain.stop();
}


//--------- Main auton functions ---------//
void auton()
{
  DrivetrainInertial.calibrate();
  Drivetrain.setDriveVelocity(25, pct);
  
  Thrower.setVelocity(35, velocityUnits::pct);
  Thrower.spin(directionType::fwd);
  wait(20, sec);
  Thrower.stop();

  Thrower.setVelocity(90, velocityUnits::pct);
  Thrower.spin(directionType::fwd);
  wait(650, msec);
  Thrower.stop(hold);
  
  Drivetrain.driveFor(reverse, 2, distanceUnits::cm);
  girarDuracion(190,20,true);
  Drivetrain.driveFor(directionType::fwd, 1, distanceUnits::cm);
  girarDuracion(500,20,false);
  Drivetrain.driveFor(reverse, 2, distanceUnits::cm);
  girarDuracion(200, 20, false);
  Drivetrain.driveFor(reverse, 17, distanceUnits::cm);


}


//--------- Main skills functions ---------//
void skill()
{
  /* Falta probar este código 
  DrivetrainInertial.calibrate();
  Drivetrain.setDriveVelocity(30, pct);
  
  Thrower.setVelocity(25, velocityUnits::pct);
  Thrower.spin(directionType::fwd);
  wait(30, sec);
  Thrower.stop();

  Thrower.setVelocity(90, velocityUnits::pct);
  Thrower.spin(directionType::fwd);
  wait(650, msec);
  Thrower.stop(hold);
  
  Drivetrain.driveFor(reverse, 2, distanceUnits::cm);
  girarDuracion(190,20,true);
  Drivetrain.driveFor(directionType::fwd, 1, distanceUnits::cm);
  girarDuracion(500,20,false);
  Drivetrain.driveFor(reverse, 2, distanceUnits::cm);
  girarDuracion(200, 20, false);
  Drivetrain.driveFor(reverse, 17, distanceUnits::cm);
  Drivetrain.driveFor(fwd, 17, distanceUnits::cm);
  girarDuracion(100,10,true);
  Drivetrain.driveFor(fwd, 3, distanceUnits::cm);*/
}