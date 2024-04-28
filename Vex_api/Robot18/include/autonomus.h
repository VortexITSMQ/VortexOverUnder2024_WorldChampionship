#include "bits/stdc++.h"
#include "robot-config.h"
#include "constants.h"
#include "vex_global.h"

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
  Blocker.setVelocity(30, velocityUnits::pct);
  Blocker.rotateTo(-15, rotationUnits::deg);
  Blocker.stop(hold);
  
  Drivetrain.setDriveVelocity(10, pct);

  Climber.setVelocity(25, velocityUnits::pct);
  Climber.spin(directionType::rev);
  wait(3, sec);
  Climber.stop();
  
  Thrower.setVelocity(35, velocityUnits::pct);
  Thrower.rotateFor(360 * 28, rotationUnits::deg); //28 para qeu de 14 putazos
  Thrower.rotateFor(230, rotationUnits::deg);
  Thrower.stop(hold);

  Drivetrain.driveFor(fwd, 2, distanceUnits::cm);
  girarDuracion(190,13,true);
  Drivetrain.driveFor(reverse, 3, distanceUnits::cm);
  girarDuracion(400,13,true);
  Drivetrain.driveFor(reverse, 2, distanceUnits::cm);
  girarDuracion(200, 13, true);//200
  Drivetrain.driveFor(reverse, 5, distanceUnits::cm);
  wait(50, msec);
  girarDuracion(300, 13, true);
  Drivetrain.setDriveVelocity(100, pct);
  Drivetrain.driveFor(reverse, 13, distanceUnits::cm);

}


//--------- Main skills functions ---------//
void skill()
{
  /*------------------------------
  El ratchet se quita del climber
  ---------------------------------*/
  Blocker.setVelocity(30, velocityUnits::pct);
  Blocker.rotateTo(-15, rotationUnits::deg);
  Blocker.stop(hold);
  
  Drivetrain.setDriveVelocity(10, pct);

  /*---------------------
  El climber se levanta
  -----------------------*/
  Climber.setVelocity(25, velocityUnits::pct);
  Climber.spin(directionType::rev);
  wait(3, sec);
  Climber.stop();
  Blocker.stop(coast); //El ratchet se regresa
  
  /*-----------------------------------
  El lanzador se activa y de 14 golpes
  -------------------------------------*/
  Thrower.setVelocity(55, velocityUnits::pct);
  //Thrower.setTimeout(3, sec);
  Thrower.rotateFor(360 * 28, rotationUnits::deg); //28 para qeu de 14 putazos
  Thrower.rotateFor(215, rotationUnits::deg);
  Thrower.stop(hold);

  /*-----------------------------------------------------
  El robot limpia tantito los triballs qeu tiene enfrente
  -------------------------------------------------------*/
  Drivetrain.driveFor(reverse, 3, distanceUnits::cm);
  Drivetrain.driveFor(fwd, 3, distanceUnits::cm);

  /*-----------------------------------
  El lanzador se activa y de 14 golpes
  -------------------------------------*/
  Thrower.setVelocity(55, velocityUnits::pct);
  //Thrower.setTimeout(3, sec);
  Thrower.rotateFor(360 * 28, rotationUnits::deg); //28 para qeu de 14 putazos
  Thrower.stop(hold);
  
  /*--------------------
  El robto empuja todo
  ----------------------*/
  Drivetrain.driveFor(fwd, 2, distanceUnits::cm);
  girarDuracion(190,13,true);
  Drivetrain.driveFor(reverse, 3, distanceUnits::cm);
  girarDuracion(400,13,true);
  Drivetrain.driveFor(reverse, 2, distanceUnits::cm);
  girarDuracion(200, 13, true);//200
  Drivetrain.driveFor(reverse, 5, distanceUnits::cm);
  wait(50, msec);
  girarDuracion(300, 13, true);
  Drivetrain.setDriveVelocity(100, pct);
  Drivetrain.driveFor(reverse, 13, distanceUnits::cm);
}