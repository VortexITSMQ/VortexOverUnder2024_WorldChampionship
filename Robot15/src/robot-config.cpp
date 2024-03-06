#include "vex.h"
#include "robot-config.h"
#include "constants.h"
#include "PID.h"
using namespace vex;

bool WingAreOpen = false;

// Cerebelo
brain Brain;

// Control (mando)
controller Controller1 = controller(primary);

// Chassis
//Motor rojo  ratio18_1
//Motor verde ratio36_1
//Motor azul  ratio6_1
inertial DrivetrainInertial = inertial(PORT11);
motor RightDriveA = motor(PORT3, ratio36_1, false);
motor RightDriveB = motor(PORT4, ratio36_1, false);
//motor RightDriveC = motor(PORT10, ratio36_1, true);

motor LeftDriveA = motor(PORT1, ratio36_1, true);
motor LeftDriveB = motor(PORT2, ratio36_1, true);
//motor LeftDriveC = motor(PORT7, ratio36_1, false);

motor_group LeftDriveSmart = motor_group(LeftDriveA, LeftDriveB);
motor_group RightDriveSmart = motor_group(RightDriveA, RightDriveB);

//Climber
motor ClimbRight = motor(PORT9, ratio36_1, false);
motor ClimbLeft = motor(PORT10, ratio36_1, true);

/*
smartdrive Drivetrain = smartdrive(LeftDriveSmart, RightDriveSmart, DrivetrainInertial, 
  WHEEL_TRAVEL, TRACK_WIDTH, TRACK_BASE, mm, EXT_GEAR_RATIO);
*/

/*
// Climber
motor ClimberLeft = motor(PORT12, ratio36_1, false);
motor ClimberRight = motor(PORT9, ratio36_1, true);
motor_group Climber = motor_group(ClimberLeft, ClimberRight);
*/


//Controller options 
bool RemoteControlCodeEnabled = true;
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;

/*
void Climber_fwd_cb(){
  while (Controller1.ButtonUp.pressing())
    Climber.spin(reverse, 90, percent);
  Climber.stop();
}

void Climber_bwd_cb(){
  while (Controller1.ButtonDown.pressing())
    Climber.spin(fwd, 90, percent);
  Climber.stop();
}
*/

void Climber_fwd_cb(){
  while (Controller1.ButtonUp.pressing()){
    ClimbRight.spin(fwd, 90, percent);
    ClimbLeft.spin(fwd, 90, percent);
  }
  ClimbRight.stop();
  ClimbLeft.stop();

}

void Climber_bwd_cb(){
  while (Controller1.ButtonDown.pressing()){
    ClimbRight.spin(reverse, 90, percent);
    ClimbLeft.spin(reverse, 90, percent);
  }
  ClimbRight.stop();
  ClimbLeft.stop();
}

/*--------------------------------------------------------------------------*/
/*                   rc_auto_loop_function_Controller1()                    */
/*                                                                          */
/*               Aquí se junta todas las funciones de control               */
/*               se repetiran una vez el control se inicialice              */
/*--------------------------------------------------------------------------*/
int rc_auto_loop_function_Controller1() {

  task billWithTheScienceFi(drivePID);

  //Funciones de botones y sistemas

  //Controller1.ButtonB.pressed(Wings_cb);
  //Controller1.ButtonX.pressed(Wings_cb);

  Controller1.ButtonUp.pressed(Climber_fwd_cb);
  Controller1.ButtonDown.pressed(Climber_bwd_cb);
  while(true) {
    chassis_control();
  }
    
  wait(20, msec);
  return 0;
}

/*--------------------------------------------------------------------------*/
/*                               vexcodeInit()                              */
/*                                                                          */
/*               Aquí solamente se calibra el sensor inercial               */
/*--------------------------------------------------------------------------*/
void vexcodeInit( void ) {
  wait(200, msec);
  DrivetrainInertial.calibrate();
  while (DrivetrainInertial.isCalibrating()) {
    wait(25, msec);
  }
  wait(50, msec);
}


/*--------------------------------------------------------------------------*/
/*                             chassis_control()                            */
/*                                                                          */
/*       Aquí se especifica la configuración de como controlar el robot     */
/*                 SI SE QUIERE CAMBIAR LA VELOCIDAD DEL ROBOT              */
/*                     se tiene que cambiar el cálculo de:                  */
/*           drivetrainLeftSideSpeed y de drivetrainRightSideSpeed          */
/*--------------------------------------------------------------------------*/
void chassis_control(){
  //DESCOMENTAR ESTO PARA UNA VELOCIDAD MANEJABLE MEDIO LENTA MEDIO RAPIDA
  //int drivetrainLeftSideSpeed = (Controller1.Axis3.position() + (0.7*Controller1.Axis1.position()))/2;
  //int drivetrainRightSideSpeed = (Controller1.Axis3.position() - (0.7*Controller1.Axis1.position()))/2;

  int drivetrainLeftSideSpeed = (Controller1.Axis3.position() + (Controller1.Axis1.position()));
  int drivetrainRightSideSpeed = (Controller1.Axis3.position() - (Controller1.Axis1.position()));
  
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