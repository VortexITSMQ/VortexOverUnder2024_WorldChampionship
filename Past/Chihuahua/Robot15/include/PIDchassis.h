#include "vex.h"
#include "robot-config.h"
#include "bits/stdc++.h"
#include<iostream>
//using namespace vex;
using namespace std;

double kP = 0.0;
double kI = 0.0;
double kD = 0.0;

double turnkP = 0.0;
double turnkI = 0.0;
double turnkD = 0.0;

int error; //SensorValue - DesireValue : Position
int prevError = 0; //Position 20 miliseconds ago
int derivative; // error - prevError : Speed
int totalError = 0; 
int desiredValue = 200;

int turnerror; //SensorValue - DesireValue : Position
int turnprevError = 0; //Position 20 miliseconds ago
int turnderivative; // error - prevError : Speed
int turntotalError = 0; 
int turndesiredValue = 200;

//Variable modified for use
bool enableDrivePID = true;
bool resetDriveSensors = false;

int drivePIDTurn(){
  double TurnMotorPower;
  double lateralMotorPower;

  while(enableDrivePID){
    if (resetDriveSensors){
      resetDriveSensors = false;
      RightDriveA.setPosition(0, degrees);
      LeftDriveA.setPosition(0, degrees);
      RightDriveB.setPosition(0, degrees);
      LeftDriveB.setPosition(0, degrees);
      
    }

    int RightDriveAPosition = RightDriveA.position(degrees);
    int RightDriveBPosition = RightDriveB.position(degrees);
    int LeftDriveAPosition = LeftDriveA.position(degrees);
    int LeftDriveBPosition = LeftDriveB.position(degrees);

    int averagePosition = (((RightDriveAPosition + RightDriveBPosition) / 2) + ((LeftDriveAPosition + LeftDriveBPosition) / 2)) / 2;
    //int averagePosition((RightDriveAPosition + LeftDriveAPosition) / 2);

    error = averagePosition - desiredValue;
    //derivative = error - prevError;
    //totalError += error;

    int turnDiff = ((RightDriveAPosition + RightDriveBPosition) / 2) - ((LeftDriveAPosition + LeftDriveBPosition) / 2);

    turnerror = turnDiff - turndesiredValue;
    turnderivative = turnerror - turnprevError;
    turntotalError += turnerror;

    TurnMotorPower = turnerror * turnkP + turnderivative * turnkD + turntotalError * turnkI;

    //int averagePosition = ();
    
    //code
    prevError = error;
    task::sleep(20);

  } 

  return TurnMotorPower;
}

int drivePIDLateral(){
  double lateralMotorPower;

  while(enableDrivePID){
    if (resetDriveSensors){
      resetDriveSensors = false;
      RightDriveA.setPosition(0, degrees);
      LeftDriveA.setPosition(0, degrees);
      RightDriveB.setPosition(0, degrees);
      LeftDriveB.setPosition(0, degrees);
      
    }

    int RightDriveAPosition = RightDriveA.position(degrees);
    int RightDriveBPosition = RightDriveB.position(degrees);
    int LeftDriveAPosition = LeftDriveA.position(degrees);
    int LeftDriveBPosition = LeftDriveB.position(degrees);

    int averagePosition = (((RightDriveAPosition + RightDriveBPosition) / 2) + ((LeftDriveAPosition + LeftDriveBPosition) / 2)) / 2;
    //int averagePosition((RightDriveAPosition + LeftDriveAPosition) / 2);

    error = averagePosition - desiredValue;
    derivative = error - prevError;
    totalError += error;

    lateralMotorPower = error * kP + derivative * kD + totalError * kI;

    //int averagePosition = ();
    
    //code
    prevError = error;
    task::sleep(20);

  } 

  return lateralMotorPower;

}