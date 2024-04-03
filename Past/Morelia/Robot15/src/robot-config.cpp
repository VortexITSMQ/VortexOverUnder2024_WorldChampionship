#include "vex.h"
#include "constants.h"
#include "vex_global.h"
#include "vex_motorgroup.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

bool WingAreOpen = false;
bool ClimberIsActive = false;

// A global instance of brain used for printing to the V5 Brain screen

brain  Brain;
controller Controller1 = controller(primary);

// Climber
motor ClimberLeft = motor(PORT16, ratio36_1, false);
motor ClimberRight = motor(PORT10, ratio36_1, true);
motor_group Climber = motor_group(ClimberLeft, ClimberRight);

// Intake
motor Collector = motor(PORT11, ratio18_1, true);

// Wings 
motor Wing = motor(PORT10, ratio18_1, true);
//pneumatics Indexer = pneumatics(Brain.ThreeWirePort.B);
pneumatics IndexerRight = pneumatics(Brain.ThreeWirePort.A);
pneumatics IndexerLeft = pneumatics(Brain.ThreeWirePort.B);


// Chassis
inertial DrivetrainInertial = inertial(PORT13);
motor RightDriveA = motor(PORT4, ratio18_1, false);//false
motor RightDriveB = motor(PORT3, ratio18_1, false);//false
motor LeftDriveA = motor(PORT1, ratio18_1, true); //11
motor LeftDriveB = motor(PORT2, ratio18_1, true); //12
motor_group LeftDriveSmart = motor_group(LeftDriveA, LeftDriveB);
motor_group RightDriveSmart = motor_group(RightDriveA, RightDriveB);
smartdrive Drivetrain = smartdrive(LeftDriveSmart, RightDriveSmart, DrivetrainInertial, 
  WHEEL_TRAVEL, TRACK_WIDTH, TRACK_BASE, mm, EXT_GEAR_RATIO);

bool RemoteControlCodeEnabled = true;
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;


void Wings_cb(){
  //If the wings are open then we close them
  if (!WingAreOpen) {
    //Wing.spinToPosition(100, degrees, true);
    IndexerLeft.set(true);
    IndexerRight.set(true);
    WingAreOpen = true;
  }
  //If the wings are close then we open them
  else {
    //Wing.spinToPosition(-100, degrees, true);
    IndexerLeft.set(false);
    IndexerRight.set(false);
    WingAreOpen = false;
  }
}

void Climber_fwd_cb(){
  while (Controller1.ButtonUp.pressing())
    Climber.spin(reverse, 70, percent);
  Climber.stop();
}

void Climber_bwd_cb(){
  while (Controller1.ButtonDown.pressing())
    Climber.spin(fwd, 70, percent);
  Climber.stop();
}

void Collector_cb(){
  while(Controller1.ButtonB.pressing())
    Collector.spin(fwd);
  Collector.stop(hold);
}


int rc_auto_loop_function_Controller1() {
  Controller1.ButtonB.pressed(Wings_cb);
  Controller1.ButtonX.pressed(Wings_cb);
  Controller1.ButtonUp.pressed(Climber_fwd_cb);
  Controller1.ButtonDown.pressed(Climber_bwd_cb);
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
  int drivetrainLeftSideSpeed = (Controller1.Axis3.position() + (0.7*Controller1.Axis1.position()))/2.0;
  int drivetrainRightSideSpeed = (Controller1.Axis3.position() - (0.7*Controller1.Axis1.position()))/2.0;
  
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

