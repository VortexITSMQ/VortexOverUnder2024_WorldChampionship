/*----------------------------------------------------------------------------*/
/*    Module:       main.h                                                    */
/*    Author:       Vortex                                                    */
/*    Created:      Wed Apr 19 2023                                           */
/*    Description:  V5 project                                                */
/*----------------------------------------------------------------------------*/


#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_

#define PROS_USE_SIMPLE_NAMES
#define PROS_USE_LITERALS

#include "api.h"
#include "lemlib/api.hpp"
//#include "okapi/api.hpp"
//#include "pros/api_legacy.h"

#ifdef __cplusplus
extern "C" {
    #endif
    void autonomous(void);
    void initialize(void);
    void disabled(void);
    void competition_initialize(void);
    void opcontrol(void);
    #ifdef __cplusplus
}
#endif

#ifdef __cplusplus

//#include <iostream>

//----------------- Robot constants -----------------//
const double WHEEL_DIAMETER = 82.55;    //mm
const double WHEEL_TRAVEL   = M_PI * WHEEL_DIAMETER;
const double TRACK_WIDTH    = 230;      //mm
const double TRACK_BASE     = 200;      //mm
const double EXT_GEAR_RATIO = 1;    
const double MOTOR_MAX_SPEED = 100;     //

//----------------- Autonomous time -----------------//
const double DIST_TO_BALL_1       = 30;
const double TURN_TO_BALL_1       = 30;

//------------------ User control -------------------//
const double JOYSTICK_DEADBAND    = 10;

#endif
#endif  // _PROS_MAIN_H_

#include <math.h>
#include <corecrt_math_defines.h>