#include <iostream>
#include "robot-config.h"
#include "constants.h"
#include "vex_global.h"
#define PI 3.141592653589793238462643383279502884L

using namespace vex;


void girarDuracion(int duracion, double velocidad, bool turn_right) {
    // Configurar la velocidad de giro de los motores
    Drivetrain.setTurnVelocity(velocidad, pct);
    // Girar el robot durante la duración especificada
    (turn_right) ? Drivetrain.turn(vex::turnType::right, velocidad, vex::velocityUnits::pct) : Drivetrain.turn(vex::turnType::left, velocidad, vex::velocityUnits::pct);
    // o vex::turnType::right
    wait(duracion, msec);
    Drivetrain.stop();
}

int grados_90 = 750;
int grados_180 = 1600;

//--------- Main auton functions ---------//
void auton()
{
  DrivetrainInertial.calibrate();
  Drivetrain.setDriveVelocity(20, pct);

  //Primera ida
  Drivetrain.driveFor(fwd,30, distanceUnits::cm);

  /*-----------------------------
  * Vuelta para empujar el trible
  * -----------------------------*/      
  DrivetrainInertial.calibrate();
  girarDuracion(250,30,false);
  IndexerRight.set(true);
  girarDuracion(350,30,false);
  IndexerRight.set(false);

  /*----------------------------------
  * Se acomoda para recoger el trible
  * ----------------------------------*/
  girarDuracion(750,15,true);
  Drivetrain.driveFor(fwd,10, distanceUnits::cm);
  girarDuracion(775,15,true);
  Drivetrain.setDriveVelocity(15, pct);
  Drivetrain.driveFor(fwd,4,distanceUnits::cm);

  /*----------------
  * Recoge el trible
  * ----------------*/
  Climber.spin(forward, 45, percent);
  wait(5.8, seconds);
  Climber.stop();

  /*---------------
  * Saca el trible
  * ---------------*/
  Drivetrain.setDriveVelocity(20, pct);
  Drivetrain.driveFor(reverse,35, distanceUnits::cm, true);

  /*-----------------
  * Suelta el trible
  * -----------------*/  
  Climber.spin(reverse, 45, percent);
  wait(5, msec);
  Climber.stop();
  girarDuracion(500,15,false);
  Climber.spin(reverse, 45, percent);
  wait(5, seconds);
  Climber.stop();
  
  /*-----------------
  * Empuja el trible
  * -----------------*/
  Drivetrain.setDriveVelocity(40, pct);
  Drivetrain.driveFor(forward, 13, distanceUnits::cm);
  Drivetrain.driveFor(reverse, 10, distanceUnits::cm);
  Drivetrain.driveFor(forward, 14, distanceUnits::cm);

}


//--------- Main skills functions ---------//
void skill()
{

  /*-----------------
  * Saca tribals
  * -----------------*/
  // Thrower.spin(fwd, 35, pct);
  // wait(20, sec);
  // Thrower.stop();

  /*-----------------------------------
  * Atora el brazo para que no estorbe
  * -----------------------------------*/
  // Thrower.spin(fwd, 90, pct);
  // wait(400, msec);
  // Thrower.stop(hold);

  /*--------------------------------------------------
  * Se acomoda para girar y empujar los triballs
  * --------------------------------------------------*/
  Drivetrain.setDriveVelocity(15, pct);
  Drivetrain.driveFor(fwd, 3, distanceUnits::cm);
  girarDuracion(300,10,true);
  Drivetrain.driveFor(reverse, 14, distanceUnits::cm);
  girarDuracion(363,20,true);

  /*--------------------------------------------------
  * Empuja los triballs para que no estorben
  * --------------------------------------------------*/
  Drivetrain.setDriveVelocity(40, pct);
  Drivetrain.driveFor(reverse, 2, distanceUnits::cm);
  Drivetrain.setDriveVelocity(10, pct);
  Drivetrain.driveFor(fwd, 2, distanceUnits::cm);

  /*------------------------------------
  * Gira para empujar despues de frente
  * ------------------------------------*/
  girarDuracion(1600,15,true);


  /*------------------------------
  * Ida al otro lado de la cancha
  * ------------------------------*/
  Drivetrain.setDriveVelocity(20, pct);
  Drivetrain.driveFor(fwd, 45, distanceUnits::cm);

  /*------------------------------
  * Gira para empujar los triballs
  * ------------------------------*/
  girarDuracion(370,20,true);
  IndexerRight.set(true);
  IndexerLeft.set(true);

  Drivetrain.driveFor(fwd, 10, distanceUnits::cm);
  girarDuracion(370,20,true);
  Drivetrain.setDriveVelocity(100, pct);
  Drivetrain.driveFor(fwd, 10, distanceUnits::cm);





}