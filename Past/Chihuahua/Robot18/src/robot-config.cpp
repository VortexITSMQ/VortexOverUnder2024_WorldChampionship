#include "vex.h"
#include "constants.h"

using namespace vex;

bool ThrowerIsOn = false;

// A global instance of brain used for printing to the V5 brain screen
brain Brain;

controller Controller1 = controller(primary);

//chassis
inertial DrivetrainInertial = inertial(PORT10);
motor RightDriveA = motor(PORT2, ratio18_1, true);
motor RightDriveB = motor(PORT19, ratio18_1, true);
motor LeftDriveA = motor(PORT1, ratio18_1, false);
motor LeftDriveB = motor(PORT10, ratio18_1, false);
motor_group LeftDriveSmart = motor_group(LeftDriveA, LeftDriveB);
motor_group RightDriveSmart = motor_group(RightDriveA, RightDriveB);
smartdrive Drivetrain = smartdrive(LeftDriveSmart, RightDriveSmart, DrivetrainInertial, 
  WHEEL_TRAVEL, TRACK_WIDTH, TRACK_BASE, mm, EXT_GEAR_RATIO);

//Collector
motor Collector = motor(PORT11, ratio18_1, true);
limit CollectorButtonBack = limit(Brain.ThreeWirePort.G);

//Lanzador
motor ThrowerUp = motor(PORT1, ratio6_1, false);
motor ThrowerDown = motor(PORT11, ratio6_1, true);
motor_group Thrower = motor_group(ThrowerUp, ThrowerDown);

bool RemoteControlCodeEnabled = true;
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;


void CollectorFront(){
  wait(0.53, seconds);
  Collector.spin(reverse);
}

void CollectorBack(){
  Collector.spin(forward);
}

void Thrower_cb(){
  if (!ThrowerIsOn){
    Thrower.spin(fwd, 500, percent);
    ThrowerIsOn = true;
  }else{
    Thrower.spin(fwd, 0, percent);
    ThrowerIsOn = false;
  }
}


int rc_auto_loop_function_Controller1() {
  //Funciones de botones y sistemas
  Controller1.ButtonR2.pressed(Thrower_cb);
  // CollectorButtonFront.pressed(CollectorFront);
  CollectorButtonBack.pressed(CollectorBack);
  CollectorButtonBack.released(CollectorFront);

  while(true) {
    chassis_control();
  }
    
  wait(20, msec);
  return 0;
}

void vexcodeInit(void) {
  wait(200, msec);
  DrivetrainInertial.calibrate();
  while (DrivetrainInertial.isCalibrating()) {
    wait(25, msec);
  }
  wait(50, msec);
}

void chassis_control(){
  //int drivetrainLeftSideSpeed = (Controller1.Axis3.position() - (0.7*Controller1.Axis1.position()))/2.0;
  //int drivetrainRightSideSpeed = (Controller1.Axis3.position() + (0.7*Controller1.Axis1.position()))/2.0;

  int drivetrainLeftSideSpeed = (Controller1.Axis3.position() - (Controller1.Axis1.position()));
  int drivetrainRightSideSpeed = (Controller1.Axis3.position() + (Controller1.Axis1.position()));
  
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

