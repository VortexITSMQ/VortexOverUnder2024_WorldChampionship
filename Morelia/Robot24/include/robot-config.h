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

// Wings
 extern motor Wing;

 // Collector
extern motor LeftRail;
extern motor RightRail;
extern motor Collector;

// Driver Control Function
extern int rc_auto_loop_function_Controller1();
void  vexcodeInit( void );
void  chassis_control();

//RAIL
extern motor LeftRail;
extern motor RightRail;
extern motor_group Rail;

//COLLECTOR
extern motor CollectorFront; 
extern motor CollectorBack;

//catapult
extern limit CatapultSwitch;
extern motor CatapultLeft;
extern motor CatapultRight;
extern motor_group Catapult;


//------- Aux function definition -------//
extern void CatapultSwitch_cb();