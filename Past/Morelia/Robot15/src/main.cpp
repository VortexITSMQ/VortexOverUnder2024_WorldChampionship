/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Drivetrain           drivetrain    1, 10           
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "autonomous.h"
#include "robot-config.h"
#include "bits/stdc++.h"

using namespace vex;
competition Competition;

// define your global instances of motors and other devices here

void display_info_dinamic(){
  std::cout << DrivetrainInertial.heading() << std::endl;
  Brain.Screen.print(Drivetrain.heading());
}

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
}

void autonomous(void) {
  auton();
}

void usercontrol(void) {
  rc_auto_loop_function_Controller1();
}

int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    display_info_dinamic();
    wait(100, msec);
  }
}
