#include <iostream>
#include "robot-config.h"
#include "constants.h"
#include "vex_global.h"
#define PI 3.141592653589793238462643383279502884L

using namespace vex;

//------- Aux function definition -------//
void inertial_turn(int lower_angle, int upper_angle, int speed, bool turn_right) {
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
  //DrivetrainInertial.calibrate();
  Drivetrain.setDriveVelocity(25, pct);

  //Primera ida
  //Drivetrain.driveFor(fwd, 25, distanceUnits::cm);

  /*-----------------------------
  * Vuelta para empujar el trible
  * -----------------------------*/
  //Drivetrain.turnToHeading(-20, rotationUnits::deg, 30, rpm, true);
  //inertial_turn(175, 178, 15, false); //hacia el grado 340 (entre el grado 339 y 341), rpm: 15, Gira a la izquierda: false
  girarDuracion(750, 15, false);
  IndexerRight.set(true);
  wait(1, seconds);
  DrivetrainInertial.calibrate();
  //Drivetrain.turnToHeading(-185, rotationUnits::deg, 50, rpm, true);
  //inertial_turn(10, 12, 15, false);
  IndexerRight.set(false);

//   /*----------------------------------
//   * Se acomoda para recoger el trible
//   * ----------------------------------*/
//   Drivetrain.driveFor(reverse, 10, distanceUnits::cm);
//   Drivetrain.turnToHeading(135, rotationUnits::deg, 30, rpm, true);
//   Drivetrain.driveFor(reverse, 26, distanceUnits::cm);//25
//   Drivetrain.turnToHeading(225, rotationUnits::deg, 30, rpm, true);
//   Drivetrain.driveFor(reverse, 20, distanceUnits::cm);

//   /*----------------
//   * Recoge el trible
//   * ----------------*/
//   Climber.spin(reverse, 45, percent);
//   wait(5.8, seconds);
//   Climber.stop();

//   /*---------------
//   * Saca el trible
//   * ---------------*/
//   Drivetrain.setDriveVelocity(35, pct);
//   Drivetrain.driveFor(fwd, 90, distanceUnits::cm, true);
//   Drivetrain.setDriveVelocity(25, pct);

//   /*---------------------------------
//   * Se acomoda para soltar el trible
//   * ---------------------------------*/
//   Drivetrain.turnToHeading(180, rotationUnits::deg, 30, rpm, true);

//   /*-----------------
//   * Suelta el trible
//   * -----------------*/
//   Climber.spin(fwd, 45, percent);
//   wait(2, seconds);
//   Climber.stop();

//   /*-----------------
//   * Empuja el trible
//   * -----------------*/
//   Drivetrain.setDriveVelocity(70, pct);
//   Drivetrain.driveFor(reverse, 50, distanceUnits::cm);
//   Drivetrain.driveFor(fwd, 10, distanceUnits::cm);
//   Drivetrain.driveFor(reverse, 20, distanceUnits::cm);

//   /*-----------------------------------
//   * Se acomoda para tocar el tubo rojo
//   * -----------------------------------*/
//   Drivetrain.setDriveVelocity(45, pct);
//   Drivetrain.driveFor(fwd, 45, distanceUnits::cm);
//   Drivetrain.turnToHeading(270, rotationUnits::deg, 30, rpm, true);
//   Drivetrain.driveFor(reverse, 50, distanceUnits::cm);
//   Drivetrain.turnToHeading(345, rotationUnits::deg, 30, rpm, true);

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
  inertial_turn(89, 91, 15, false);
  //Drivetrain.turnToHeading(90, rotationUnits::deg, 90, rpm, true);
  //girarGrados(20, 20);
  //Drivetrain.driveFor(reverse, 100, distanceUnits::cm);

}