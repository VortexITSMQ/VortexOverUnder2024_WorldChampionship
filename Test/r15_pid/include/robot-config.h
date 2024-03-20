#ifndef ROBOTCONFIG
#define ROBOTCONFIG

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

// PID
extern double lateralMotorPower;
extern double TurnMotorPower;

extern double kP;
extern double kI; 
extern double kD;

extern double turnkP;
extern double turnkI;
extern double turnkD;

extern int error; //SensorValue - DesireValue : Position
extern int prevError; //Position 20 miliseconds ago
extern int derivative; // error - prevError : Speed
extern int totalError; 
extern int desiredValue;

extern int turnerror; //SensorValue - DesireValue : Position
extern int turnprevError; //Position 20 miliseconds ago
extern int turnderivative; // error - prevError : Speed
extern int turntotalError; 
extern int turndesiredValue;

extern bool enableDrivePID;
extern bool resetDriveSensors;

// Wings
extern motor Wing;

// Intake
extern motor Collector;

// Driver Control Function
extern int rc_auto_loop_function_Controller1();
extern int drivePID();
//extern void display_info1();
void vexcodeInit(void);
void chassis_control();

#endif