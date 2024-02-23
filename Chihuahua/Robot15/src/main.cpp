/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Vortex                                                    */
/*    Created:      Sat Dec 09 2023                                           */
/*    Description:  Robot 15 code                                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]         [Port(s)]
// Drivetrain           drivetrain      1, 10           
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
//#include "PIDchassis.h"
#include "autonomus.h"
#include "robot-config.h"
#include "bits/stdc++.h" //Librerias esenciales de C++
using namespace vex;
competition Competition;

/*---------------------------------------------------------------------------*/
/*                                  pre_auton                                */
/*                                                                           */
/*              ESTA ES LA CONFIGURACIÓN INICIAL DEL AUTONOMO                */
/*             Nosotros no creamos esta función, se crea solita              */
/*                            NO SE DEBE BORRAR!!                            */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
}

/*---------------------------------------------------------------------------*/
/*                               display_info                                */
/*                                                                           */
/*             Aquí solo imprimimos algo, ya sea en la consola               */
/*                                  o en cerebro                             */
/*   Es buena para imprimir valores, como grados, medidas, posiciones, etc   */
/*---------------------------------------------------------------------------*/

void display_info(){
  std::cout << "Hola desde la consola" << std::endl;
  Brain.Screen.print("Hola desde el cerebro");
}

/*---------------------------------------------------------------------------*/
/*                                autonomous                                 */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*                            a VEX Competition.                             */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  auton();
  //task billWithTheScienceFi(drivePID);

  //resetDriveSensors = true;
  //enableDrivePID = true;
  //desiredValue = 300;
  //turndesiredValue = 600;
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

void skills(){
 skill();
}

/*---------------------------------------------------------------------------*/
/*                                usercontrol                                */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*                            a VEX Competition.                             */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  //enableDrivePID = false;
  
  // User control code here, inside the loop
  rc_auto_loop_function_Controller1();
}


int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  //Competition.autonomous(skills);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    display_info(); //Es bueno imprimir para que se muestren varios valores mientras corre el robot
    wait(100, msec);
  }
}