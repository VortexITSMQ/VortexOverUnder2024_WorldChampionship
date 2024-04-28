#include "vex.h"
//#include "robot-config.h"
#include "constants.h"
#include "PID.h"
using namespace vex;

// Cerebelo
brain Brain;

// Control (mando)
controller Controller1 = controller(primary);

// Chassis
// Motor rojo  ratio18_1
// Motor verde ratio36_1
// Motor azul  ratio6_1
inertial DrivetrainInertial = inertial(PORT15);
motor LeftDriveA = motor(PORT10, ratio36_1, false);
motor LeftDriveB = motor(PORT20, ratio36_1, false);

motor RightDriveA = motor(PORT1, ratio36_1, true);
motor RightDriveB = motor(PORT12, ratio36_1, true);

motor_group LeftDriveSmart = motor_group(LeftDriveA, LeftDriveB);
motor_group RightDriveSmart = motor_group(RightDriveA, RightDriveB);


smartdrive Drivetrain = smartdrive(LeftDriveSmart, RightDriveSmart, DrivetrainInertial, 
  WHEEL_TRAVEL, TRACK_WIDTH, TRACK_BASE, mm, EXT_GEAR_RATIO);

// Collector
motor RightCollector = motor(PORT2, ratio36_1, true);
motor LeftCollector = motor(PORT9, ratio36_1, false);
motor_group Collector = motor_group(LeftCollector, RightCollector);

//Thrower
motor Thrower = motor(PORT11, ratio18_1, true);

// Climber
motor ClimberLeft = motor(PORT13, ratio18_1, false);
motor ClimberRight = motor(PORT17, ratio18_1, true);
motor_group Climber = motor_group(ClimberLeft, ClimberRight);

motor Blocker = motor(PORT18, ratio36_1, false);

// Controller options 
bool RemoteControlCodeEnabled = true;
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;

/*-----------------------------------------------------------------------------------\
  /$$$$$$            /$$ /$$                       /$$                               |
 /$$__  $$          | $$| $$                      | $$                               |
| $$  \__/  /$$$$$$ | $$| $$  /$$$$$$   /$$$$$$$ /$$$$$$    /$$$$$$   /$$$$$$        |
| $$       /$$__  $$| $$| $$ /$$__  $$ /$$_____/|_  $$_/   /$$__  $$ /$$__  $$       |
| $$      | $$  \ $$| $$| $$| $$$$$$$$| $$        | $$    | $$  \ $$| $$  \__/       |
| $$    $$| $$  | $$| $$| $$| $$_____/| $$        | $$ /$$| $$  | $$| $$             |
|  $$$$$$/|  $$$$$$/| $$| $$|  $$$$$$$|  $$$$$$$  |  $$$$/|  $$$$$$/| $$             |
 \______/  \______/ |__/|__/ \_______/ \_______/   \___/   \______/ |__/             |
------------------------------------------------------------------------------------*/

bool Collecting = false;

void Collector_fwd_cb(){
  Collector.setVelocity(100, percent);
  while (Controller1.ButtonR1.pressing()){
    Collector.spin(fwd, 90, percent);
  }
  Collector.stop();
}

void Collector_bwd_cb(){
  Collector.setVelocity(100, percent);
  while (Controller1.ButtonR2.pressing()){
    Collector.spin(reverse, 90, percent);
  }
  Collector.stop();
}


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


bool Throwing = false;

void Thrower_cb() {
  if (!Throwing) {
    Thrower.setVelocity(40, pct);
    Thrower.spin(directionType::fwd);
    Throwing = true;
  } else {
    Thrower.stop();
    Throwing = false;
  }
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
  while (Controller1.ButtonL1.pressing()){
    Climber.spin(fwd, 100, percent);
  }
  Climber.stop();
}

void Climber_bwd_cb(){
  while (Controller1.ButtonL2.pressing()){
    Climber.spin(reverse, 100, percent);
  }
  Climber.stop();
}

//----------------------------------------------------------

bool BlockerOn = false;

void Blocker_cb() {
  Blocker.setVelocity(15, velocityUnits::pct);
  if (!BlockerOn) {
    Blocker.rotateTo(-15, rotationUnits::deg);
    Blocker.stop(hold);
    BlockerOn = true;
  } else {
    Blocker.stop(coast);
    BlockerOn = false;
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
  Controller1.ButtonL1.pressed(Climber_fwd_cb);
  Controller1.ButtonL2.pressed(Climber_bwd_cb);

  Controller1.ButtonX.pressed(Blocker_cb);

  Controller1.ButtonR1.pressed(Collector_fwd_cb);
  Controller1.ButtonR2.pressed(Collector_bwd_cb);
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
  float pow = 1;
  int div = 2;
  //DESCOMENTAR ESTO PARA UNA VELOCIDAD MANEJABLE MEDIO LENTA MEDIO RAPIDA
  int drivetrainLeftSideSpeed = (Controller1.Axis3.position() - (Controller1.Axis1.position()))*pow;
  int drivetrainRightSideSpeed = (Controller1.Axis3.position() + (Controller1.Axis1.position()))*pow;
  
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