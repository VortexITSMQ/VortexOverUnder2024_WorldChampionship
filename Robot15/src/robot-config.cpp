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

motor LeftDriveA = motor(PORT2, ratio36_1, true);
motor LeftDriveB = motor(PORT1, ratio36_1, true);
//motor LeftDriveC = motor(PORT7, ratio36_1, false);

motor_group LeftDriveSmart = motor_group(LeftDriveA, LeftDriveB);
motor_group RightDriveSmart = motor_group(RightDriveA, RightDriveB);

//Collector
motor CollectorRight = motor(PORT9, ratio36_1, false);
motor CollectorLeft = motor(PORT10, ratio36_1, true);

//Thrower
motor Thrower = motor(PORT11, ratio36_1, false);

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
  while (Controller1.ButtonR1.pressing()){
    CollectorRight.spin(fwd, 90, percent);
    CollectorLeft.spin(fwd, 90, percent);
  }
  CollectorRight.stop();
  CollectorLeft.stop();

}

void Climber_bwd_cb(){
  while (Controller1.ButtonR2.pressing()){
    CollectorRight.spin(reverse, 90, percent);
    CollectorLeft.spin(reverse, 90, percent);
  }
  CollectorRight.stop();
  CollectorLeft.stop();
}

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
    Thrower.rotateTo(90, rotationUnits::deg); // Rotar hacia la posición objetivo
    Thrower.rotateTo(0, rotationUnits::deg); // Rotar hacia la posición objetivo
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

void RotateToZero() {
    if (Thrower.position(rotationUnits::deg)) { // Si el motor ha terminado de rotar
        Thrower.rotateTo(0, rotationUnits::deg); // Regresar a la posición inicial (0 grados)
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

  //Controller1.ButtonB.pressed(Wings_cb);
  //Controller1.ButtonX.pressed(Wings_cb);

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