#include "vex_motorgroup.h"
using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern smartdrive Drivetrain;

// Chassis
extern motor RightDriveA;
extern motor RightDriveB;
extern motor LeftDriveA;
extern motor LeftDriveB;
extern motor_group LeftDriveSmart;
extern motor_group RightDriveSmart;
extern inertial DrivetrainInertial;
extern motor ClimberRight;
extern motor ClimberLeft;
extern motor_group Climber;
extern pneumatics IndexerLeft;
extern pneumatics IndexerRight;

// Wings
extern motor Wing;

// Intake
extern motor Collector;

// Driver Control Function
extern int rc_auto_loop_function_Controller1();
void vexcodeInit(void);
void chassis_control();