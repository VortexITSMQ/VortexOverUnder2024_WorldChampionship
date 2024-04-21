#include <iostream>
#include "robot-config.h"
#include "constants.h"
#include "vex_global.h"
#define PI 3.141592653589793238462643383279502884L

using namespace vex;

//------- Aux function definition -------//
/*void inertial_turn(int lower_angle, int upper_angle, int speed, bool turn_right) {
    while (true) {
        double current_angle = DrivetrainInertial.heading();
        if (lower_angle < current_angle && current_angle < upper_angle) {
            Drivetrain.stop();
            break;
        }
        else {
            // Establecer la velocidad y la dirección del giro
            Drivetrain.setTurnVelocity(speed, percent);
            (turn_right) ? Drivetrain.turn(right) : Drivetrain.turn(left);
            wait(20, msec);
        }
    }
}
*/
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
  //DrivetrainInertial.calibrate();

  //Drivetrain.setDriveVelocity(90, pct);

  //***************************************
  //AQUI VA EL código del BRAZO AVENTANDO



  //***************************************

  //Drivetrain.driveFor(reverse, 10, distanceUnits::cm);
  //DrivetrainInertial.calibrate();
//inertial_turn(89, 91, 15, false);
  //Drivetrain.turnToHeading(90, rotationUnits::deg, 90, rpm, true);
  //girarGrados(20, 20);
  //Drivetrain.driveFor(reverse, 100, distanceUnits::cm);

}