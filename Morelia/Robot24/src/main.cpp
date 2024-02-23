/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Authors:      Israel Ivan Arroyo   and    Diego Emilio Barrera          */
/*    LinkedIns:                                         */
/*    Team:         VORTEX                                                    */
/*    Created:      Wed Apr 19 2023                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include "autonomous.h"
#include "robot-config.h"

using namespace vex;
competition Competition; 

void display_info_dinamic(){
  std::cout << "Im working " << std::endl;
}

void pre_auton(void) {
  vexcodeInit();
}

void autonomous(void) {
  auton();
}

void usercontrol(void) {
  rc_auto_loop_function_Controller1();
}

int main() {
  //Competition.autonomous(autonomous);
  //Competition.autonomous(skillsTest);
  Competition.autonomous(skills);


  pre_auton();

  while (true) {
    display_info_dinamic();

    wait(100, msec);
  }
}
