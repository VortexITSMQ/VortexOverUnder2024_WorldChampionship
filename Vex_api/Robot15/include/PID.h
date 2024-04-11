//#include "robot-config.h"
//----------------------------
#include "vex.h"
#include "robot-config.h"
#include "bits/stdc++.h"
#include<iostream>


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
    //int averagePosition = ();
    
    //code
    prevError = error;
    task::sleep(20);
  } 
  return -1;
}