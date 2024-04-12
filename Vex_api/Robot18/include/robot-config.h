using namespace vex;

extern brain Brain;
extern inertial DrivetrainInertial;
extern controller Controller1;
extern smartdrive Drivetrain;

//chassis
extern motor RightDriveA;
extern motor RightDriveB;

extern motor LeftDriveA;
extern motor LeftDriveB;

extern motor_group LeftDriveSmart;
extern motor_group RightDriveSmart;
extern smartdrive Drivetrain;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void);
extern int rc_auto_loop_function_Controller1();
void chassis_control();
