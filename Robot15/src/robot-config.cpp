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
// Motor rojo  ratio18_1
// Motor verde ratio36_1
// Motor azul  ratio6_1
inertial DrivetrainInertial = inertial(PORT5);
motor RightDriveA = motor(PORT12, ratio36_1, false);
motor RightDriveB = motor(PORT2, ratio36_1, false);

motor LeftDriveA = motor(PORT19, ratio36_1, true);
motor LeftDriveB = motor(PORT9, ratio36_1, true);

motor_group LeftDriveSmart = motor_group(LeftDriveA, LeftDriveB);
motor_group RightDriveSmart = motor_group(RightDriveA, RightDriveB);

/*
smartdrive Drivetrain = smartdrive(LeftDriveSmart, RightDriveSmart, DrivetrainInertial, 
  WHEEL_TRAVEL, TRACK_WIDTH, TRACK_BASE, mm, EXT_GEAR_RATIO);
*/

//Thrower
motor Thrower = motor(PORT11, ratio36_1, false);

// Climber
motor ClimberLeft = motor(PORT1, ratio18_1, false);
motor ClimberRight = motor(PORT10, ratio18_1, true);

// Wings
pneumatics IndexerRight = pneumatics(Brain.ThreeWirePort.B);
pneumatics IndexerLeft = pneumatics(Brain.ThreeWirePort.B);

// Controller options 
bool RemoteControlCodeEnabled = true;
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;

/*-----------------------------------------------------------------------------------\
 /$$$$$$$$ /$$                                                                       |
|__  $$__/| $$                                                                       |
   | $$   | $$$$$$$   /$$$$$$   /$$$$$$  /$$  /$$  /$$  /$$$$$$   /$$$$$$            |
   | $$   | $$__  $$ /$$__  $$ /$$__  $$| $$ | $$ | $$ /$$__  $$ /$$__  $$           |
   | $$   | $$  \ $$| $$  \__/| $$  \ $$| $$ | $$ | $$| $$$$$$$$| $$  \__/           |
   | $$   | $$  | $$| $$      | $$  | $$| $$ | $$ | $$| $$_____/| $$                 |
   | $$   | $$  | $$| $$      |  $$$$$$/|  $$$$$/$$$$/|  $$$$$$$| $$                 |
   |__/   |__/  |__/|__/       \______/  \_____/\___/  \_______/|__/                 |
------------------------------------------------------------------------------------*/

int target_position = 0; // Posición objetivo inicial (0 grados)
bool Throwing = false;

void Thrower_cb() {
    // Si se presiona el botón A y no se está lanzando actualmente
  // if (Controller1.ButtonR1.pressing() && !Throwing) {
  //   while(true){
  //     Thrower.rotateTo(90, rotationUnits::deg); // Rotar hacia la posición objetivo
  //     Thrower.rotateTo(0, rotationUnits::deg); // Rotar hacia la posición objetivo
  //     Throwing = true;
  //     if(Controller1.ButtonR1.pressing()){
  //       Throwing = false;
  //     }
  //   }
  // }
  if(Controller1.ButtonA.pressing()){
      Throwing = true;
  }
  do{
    if(Controller1.ButtonA.pressing()){
      Throwing = false;
    }
    Thrower.setVelocity(100, percent);
    // Thrower.rotateTo(90, rotationUnits::deg); // Rotar hacia la posición objetivo
    // Thrower.rotateTo(0, rotationUnits::deg); // Rotar hacia la posición objetivo
  }while(Throwing == true);
  

  // if(Controller1.ButtonA.pressing()){
  //   Throwing = !Throwing;
  // }
  // while (Throwing){
  //   Thrower.rotateTo(90, rotationUnits::deg);
  //   Thrower.rotateTo(0, rotationUnits::deg);
  //   if(Controller1.ButtonA.pressing()){
  //     Thrower.rotateTo(0, rotationUnits::deg);
  //     Throwing = false;
  //   }
  // }
}


/*-----------------------------------------------------------------------------------\
  /$$$$$$  /$$ /$$               /$$                                                 |
 /$$__  $$| $$|__/              | $$                                                 |
| $$  \__/| $$ /$$ /$$$$$$/$$$$ | $$$$$$$   /$$$$$$   /$$$$$$                        |
| $$      | $$| $$| $$_  $$_  $$| $$__  $$ /$$__  $$ /$$__  $$                       |
| $$      | $$| $$| $$ \ $$ \ $$| $$  \ $$| $$$$$$$$| $$  \__/                       |
| $$    $$| $$| $$| $$ | $$ | $$| $$  | $$| $$_____/| $$                             |
|  $$$$$$/| $$| $$| $$ | $$ | $$| $$$$$$$/|  $$$$$$$| $$                             |
 \______/ |__/|__/|__/ |__/ |__/|_______/  \_______/|__/                             |
------------------------------------------------------------------------------------*/

void Climber_fwd_cb(){
  while (Controller1.ButtonR1.pressing()){
    ClimberRight.spin(fwd, 90, percent);
    ClimberLeft.spin(fwd, 90, percent);
  }
  ClimberRight.stop();
  ClimberLeft.stop();
}

void Climber_bwd_cb(){
  while (Controller1.ButtonR2.pressing()){
    ClimberRight.spin(reverse, 90, percent);
    ClimberLeft.spin(reverse, 90, percent);
  }
  ClimberRight.stop();
  ClimberLeft.stop();
}


/*-----------------------------------------------------------------------------------\
 /$$      /$$ /$$                                                                    |
| $$  /$ | $$|__/                                                                    |
| $$ /$$$| $$ /$$ /$$$$$$$   /$$$$$$   /$$$$$$$                                      |
| $$/$$ $$ $$| $$| $$__  $$ /$$__  $$ /$$_____/                                      |
| $$$$_  $$$$| $$| $$  \ $$| $$  \ $$|  $$$$$$                                       |
| $$$/ \  $$$| $$| $$  | $$| $$  | $$ \____  $$                                      |
| $$/   \  $$| $$| $$  | $$|  $$$$$$$ /$$$$$$$/                                      |
|__/     \__/|__/|__/  |__/ \____  $$|_______/                                       |
                            /$$  \ $$                                                |
                           |  $$$$$$/                                                |
                            \______/                                                 |
------------------------------------------------------------------------------------*/

void Wings_cb(){
  //If the wings are open then we close them
  if (!WingAreOpen) {
    // Wing.spinToPosition(100, degrees, true);
    IndexerLeft.set(true);
    IndexerRight.set(true);
    WingAreOpen = true;
  }
  // If the wings are close then we open them
  else {
    // Wing.spinToPosition(-100, degrees, true);
    IndexerLeft.set(false);
    IndexerRight.set(false);
    WingAreOpen = false;
  }
}

void RotateToZero() {
    if (Thrower.position(rotationUnits::deg)) { // Si el motor ha terminado de rotar
      // Thrower.rotateTo(0, rotationUnits::deg); // Regresar a la posición inicial (0 grados)
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
  Controller1.ButtonB.pressed(Wings_cb);
  Controller1.ButtonX.pressed(Wings_cb);
  Controller1.ButtonR1.pressed(Climber_fwd_cb);
  Controller1.ButtonR2.pressed(Climber_bwd_cb);
  Controller1.ButtonA.pressed(Thrower_cb);
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
  float pow = 0.7;
  int div = 2;
  //DESCOMENTAR ESTO PARA UNA VELOCIDAD MANEJABLE MEDIO LENTA MEDIO RAPIDA
  int drivetrainLeftSideSpeed = (Controller1.Axis3.position() + (pow*Controller1.Axis1.position()))/div;
  int drivetrainRightSideSpeed = (Controller1.Axis3.position() - (pow*Controller1.Axis1.position()))/div;
  
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