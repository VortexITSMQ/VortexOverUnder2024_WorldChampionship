#include "vex.h"
#include "constants.h"
#include "robot-config.h"
#include "bits/stdc++.h"
using namespace vex;

bool WingAreOpen = false;

// Cerebelo
brain Brain;

// Control (mando)
controller Controller1 = controller(primary);

// Chassis
//Motor rojo  18:1
//Motor verde 36:1
//Motor azul   6:1
inertial DrivetrainInertial = inertial(PORT13);
motor RightDriveA = motor(PORT4, ratio18_1, false);
motor RightDriveB = motor(PORT3, ratio18_1, false);
motor LeftDriveA = motor(PORT1, ratio18_1, true);
motor LeftDriveB = motor(PORT2, ratio18_1, true);
motor_group LeftDriveSmart = motor_group(LeftDriveA, LeftDriveB);
motor_group RightDriveSmart = motor_group(RightDriveA, RightDriveB);
smartdrive Drivetrain = smartdrive(LeftDriveSmart, RightDriveSmart, DrivetrainInertial, 
  WHEEL_TRAVEL, TRACK_WIDTH, TRACK_BASE, mm, EXT_GEAR_RATIO);

// Wings 
motor Wing = motor(PORT10, ratio18_1, true);
pneumatics IndexerBoth = pneumatics(Brain.ThreeWirePort.A);

bool RemoteControlCodeEnabled = true;
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;

// PID gains for lateral movement
double kP = 0.0;
double kI = 0.0;
double kD = 0.0;

// PID gains for turning movement
double turnkP = 0.0;
double turnkI = 0.0;
double turnkD = 0.0;

// Errors compared to reference value for lateral movement
int error; //SensorValue - DesireValue : Position
int prevError = 0; //Position 20 miliseconds ago
int derivative; // error - prevError : Speed
int totalError = 0; 

// This variable won't be used, since the reference value comes from joystick
int desiredValue;

// Errors compared to reference value for turning movement
int turnerror; //SensorValue - DesireValue : Position
int turnprevError = 0; //Position 20 miliseconds ago
int turnderivative; // error - prevError : Speed
int turntotalError = 0; 
int turndesiredValue;

//Variable modified for use
bool enableDrivePID = true;
bool resetDriveSensors = true;

// Velocities for output
double lateralMotorPower;
double TurnMotorPower;

/*--------------------------------------------------------------------------*/
/*                                 drivePID()                               */
/*                                                                          */
/*               Implementation of PID controller for chassis               */
/*                       movement, lateral and turning.                     */
/*--------------------------------------------------------------------------*/

int drivePID(){
  while(enableDrivePID){
    if (resetDriveSensors){
      resetDriveSensors = false;
      RightDriveA.setPosition(0, degrees);
      LeftDriveA.setPosition(0, degrees);
      RightDriveB.setPosition(0, degrees);
      LeftDriveB.setPosition(0, degrees);
      
    }

    int RightDriveAVel = RightDriveA.velocity(pct); // [0, 100]
    int LeftDriveAVel = LeftDriveA.velocity(pct);

    int averagePosition = ((RightDriveAVel + LeftDriveAVel) / 2);
    //int averagePosition((RightDriveAPosition + LeftDriveAPosition) / 2);

    error = averagePosition - desiredValue;
    derivative = error - prevError;
    totalError += error;

    lateralMotorPower = error * kP + derivative * kD + totalError * kI;


    int turnDiff = (RightDriveAVel - LeftDriveAVel);

    turnerror = turnDiff - turndesiredValue;
    turnderivative = turnerror - turnprevError;
    turntotalError += turnerror;

    TurnMotorPower = turnerror * turnkP + turnderivative * turnkD + turntotalError * turnkI;
    //Brain.Screen.print("hola ");

    //int averagePosition = ();
    
    //code
    prevError = error;
    task::sleep(20);

  } 
  return -1;
}


void Wings_cb(){
  std::cout << lateralMotorPower << std::endl;
  //If the wings are open then we close them
  if (!WingAreOpen) {
    //Wing.spinToPosition(100, degrees, true);
    IndexerBoth.set(true);
    WingAreOpen = true;
  }
  //If the wings are close then we open them
  else {
    //Wing.spinToPosition(-100, degrees, true);
    IndexerBoth.set(false);
    WingAreOpen = false;
  }
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
  //..............................
  //..............................

  Controller1.ButtonB.pressed(Wings_cb);
  Controller1.ButtonX.pressed(Wings_cb);
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
  //int drivetrainLeftSideSpeed = (Controller1.Axis3.position() - (0.7*Controller1.Axis1.position()))/2.0;
  //int drivetrainRightSideSpeed = (Controller1.Axis3.position() + (0.7*Controller1.Axis1.position()))/2.0;

  desiredValue = Controller1.Axis3.position();
  turndesiredValue = Controller1.Axis1.position();

  int drivetrainLeftSideSpeed = (lateralMotorPower + turndesiredValue);
  int drivetrainRightSideSpeed = (lateralMotorPower - turndesiredValue);
  
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