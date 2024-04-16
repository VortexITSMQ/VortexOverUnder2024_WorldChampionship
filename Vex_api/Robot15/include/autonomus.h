#include <iostream>
#include "robot-config.h"
#include "constants.h"
#include "vex_global.h"
#define PI 3.141592653589793238462643383279502884L

using namespace vex;

//--------- Main auton functions ---------//
void auton()
{
  //DrivetrainInertial.calibrate();
  Drivetrain.setDriveVelocity(25, pct);

  Drivetrain.turnToHeading(20, rotationUnits::deg, 50, rpm, true);

}

void girarDuracion(int duracion, double velocidad) {
    // Configurar la velocidad de giro de los motores
    Drivetrain.setTurnVelocity(velocidad, pct);
    // Girar el robot durante la duración especificada
    Drivetrain.turn(vex::turnType::left, velocidad, vex::velocityUnits::pct); // o vex::turnType::right
    wait(duracion, msec);
    Drivetrain.stop();
}

//------- Aux function definition -------//
void inertial_turn(int lower_angle, int upper_angle, int velocity, bool turn_right) {
    while (true) {
        double current_angle = DrivetrainInertial.heading();
        if (lower_angle < current_angle && current_angle < upper_angle) {
            Drivetrain.stop();
            break;
        }
        else {
            Drivetrain.setTurnVelocity(velocity, percent);
            if (turn_right) {
                Drivetrain.turn(left);
            } else {
                Drivetrain.turn(right);
            }
            wait(20, msec);
        }
    }
}




//--------- Main skills functions ---------//
void skill()
{
  //DrivetrainInertial.calibrate();

  Drivetrain.setDriveVelocity(10, pct);

  //***************************************
  //AQUI VA EL código del BRAZO AVENTANDO



  //***************************************

  //Drivetrain.driveFor(reverse, 10, distanceUnits::cm);
  //DrivetrainInertial.calibrate();
  inertial_turn(89, 91, 15, false);
  //Drivetrain.turnToHeading(20, rotationUnits::deg, 5, rpm, true);
  //girarGrados(20, 20);
  //Drivetrain.driveFor(reverse, 100, distanceUnits::cm);

}