#include "vex.h"
#include "constants.h"


using namespace vex;
using signature = vision::signature;
using code = vision::code;

// Global varible that checks if the wings are open
 bool WingAreOpen = false;
 bool RailActive = false;
 bool LilArmOpen = false;

//---------------------- Devices ----------------------//
brain  Brain;
controller Controller1 = controller(primary);
// Catapult
limit CatapultSwitch = limit(Brain.ThreeWirePort.C);
motor CatapultLeft = motor(PORT20, ratio36_1, false);
motor CatapultRight = motor(PORT10, ratio36_1, false);
motor_group Catapult = motor_group(CatapultLeft, CatapultRight);
// Recolector
motor LeftRail = motor(PORT12, ratio18_1, false);
motor RightRail = motor(PORT5, ratio18_1, true);
motor_group Rail = motor_group(LeftRail, RightRail);
motor CollectorFront = motor(PORT13, ratio18_1, false); 
motor CollectorBack = motor(PORT3, ratio18_1, true);
// Wings
pneumatics IndexerRight = pneumatics(Brain.ThreeWirePort.A);
pneumatics IndexerLeft = pneumatics(Brain.ThreeWirePort.B);
// lil arm
motor LilArm = motor(PORT10, ratio18_1, true);
// Chassis
inertial DrivetrainInertial = inertial(PORT1);
motor RightDriveA = motor(PORT2, ratio18_1, true);
motor RightDriveB = motor(PORT9, ratio18_1, true);
motor LeftDriveA  = motor(PORT11, ratio18_1, false);
motor LeftDriveB  = motor(PORT19, ratio18_1, false);
motor_group LeftDriveSmart = motor_group(LeftDriveA, LeftDriveB);
motor_group RightDriveSmart = motor_group(RightDriveA, RightDriveB);
smartdrive Drivetrain = smartdrive(LeftDriveSmart, RightDriveSmart, DrivetrainInertial, 
  WHEEL_TRAVEL, TRACK_WIDTH, TRACK_BASE, mm, EXT_GEAR_RATIO);


//---------------------- User control ----------------------//
bool RemoteControlCodeEnabled = true;
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;

void LilArm_cb(){
  //If the wings are open then we close them
  if (!LilArmOpen) {
    LilArm.spinToPosition(100, degrees, true);
    LilArmOpen = true;
  }
  //If the wings are close then we open them
  else {
    LilArm.spinToPosition(-100, degrees, true);
    LilArmOpen = false;
  }
}

void Wings_cb(){
  //If the wings are open then we close them
  if (!WingAreOpen) {
    IndexerLeft.open();
    IndexerRight.open();
    WingAreOpen = true;
  }
  //If the wings are closed then we open them
  else {
    IndexerLeft.close();
    IndexerRight.close();
    WingAreOpen = false;
  }
}

void Rail_fwd_cb(){
  while(Controller1.ButtonL1.pressing())
    Rail.spin(reverse, 90, percent);
  Rail.stop(hold);
}

void Rail_bwd_cb(){
  while(Controller1.ButtonL2.pressing())
    Rail.spin(fwd, 90, percent);
  Rail.stop(hold);
}

void Collector_fwd_cb(){
  while(Controller1.ButtonX.pressing()){
    CollectorFront.spin(fwd);
    CollectorBack.spin(fwd, 80, percent);
  }
  CollectorFront.stop();
  CollectorBack.stop();
}

void Collector_bwd_cb(){
  while(Controller1.ButtonY.pressing()){
    CollectorFront.spin(reverse);
    CollectorBack.spin(reverse, 80, percent);
  }
  CollectorFront.stop();
  CollectorBack.stop();
}

void CatapultSwitch_cb(){
  Catapult.stop(hold);
  printf("catapultswitch\n");
}

void ReleaseCatapult_cb(){
  Catapult.spin(fwd, 40, rpm);
}

int rc_auto_loop_function_Controller1() {
  Controller1.ButtonA.pressed(Wings_cb);
  // Controller1.ButtonR2.pressed(LilArm_cb);
  Controller1.ButtonL1.pressed(Rail_fwd_cb);
  Controller1.ButtonL2.pressed(Rail_bwd_cb);
  Controller1.ButtonX.pressed(Collector_fwd_cb);
  Controller1.ButtonY.pressed(Collector_bwd_cb);
  CatapultSwitch.pressed(CatapultSwitch_cb);
  Controller1.ButtonUp.pressed(ReleaseCatapult_cb);
  Catapult.spin(fwd, 30, rpm);
  while(true) {
    chassis_control();
  }
    
  wait(20, msec);
  return 0;
}

void vexcodeInit( void ) {
  wait(200, msec);
  DrivetrainInertial.calibrate();
  while (DrivetrainInertial.isCalibrating()) {
    wait(25, msec);
  }
  wait(50, msec);
}

void chassis_control(){
  int drivetrainLeftSideSpeed = -(Controller1.Axis3.position() - Controller1.Axis1.position());
  int drivetrainRightSideSpeed = -(Controller1.Axis3.position() + Controller1.Axis1.position());
  if (drivetrainLeftSideSpeed < JOYSTICK_DEADBAND && drivetrainLeftSideSpeed > -JOYSTICK_DEADBAND) {
    if (DrivetrainLNeedsToBeStopped_Controller1) {
      LeftDriveSmart.stop();
      DrivetrainLNeedsToBeStopped_Controller1 = false;
    }
  } else {
    DrivetrainLNeedsToBeStopped_Controller1 = true;
  }
  if (drivetrainRightSideSpeed < JOYSTICK_DEADBAND && drivetrainRightSideSpeed > -JOYSTICK_DEADBAND) {
    if (DrivetrainRNeedsToBeStopped_Controller1) {
      RightDriveSmart.stop();
      DrivetrainRNeedsToBeStopped_Controller1 = false;
    }
  } else {
    DrivetrainRNeedsToBeStopped_Controller1 = true;
  }
  
  if (DrivetrainLNeedsToBeStopped_Controller1) {
    LeftDriveSmart.setVelocity(drivetrainLeftSideSpeed, percent);
    LeftDriveSmart.spin(forward);
  }
  if (DrivetrainRNeedsToBeStopped_Controller1) {
    RightDriveSmart.setVelocity(drivetrainRightSideSpeed, percent);
    RightDriveSmart.spin(forward);
  }
}