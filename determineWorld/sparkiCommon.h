#ifndef _INCL_SPARKIC
#define _INCL_SPARKIC

#define USE_LCD false

// Ultrasound position info
#define ULTRASOUND_FORWARD_OF_CENTER 4.2
#define ULTRASOUND_BEHIND_FRONT 1.3

// Adjustment for ultrasound servo to have it at position 0
// (mine was 5 degrees off)
#define ULTRASOUND_SERVO_ANGLE_ADJUSTMENT 5

// Max effective range of ultrasound 6 feet (~183cm)
#define ULTRASOUND_MAX_RANGE 183

// Gripper characterisitics
#define GRIPPER_LENGTH 2.6
#define GRIPPER_MAX_WIDTH 4.0

// Velocity for movement
#define VELOCITY_CM_P_SEC 2.78

// Sparki characteristics (length/width), note the axle is in the center
// so the nose length is equal to it's tail :)
#define OVERALL_WIDTH 10.5
#define OVERALL_LENGTH_LESS_GRIPPER 11.28

// When taking ultrasound measurements it's the sample size to use and
// the delay between them
#define ULTRASOUND_SAMPLE_DELAY 5
#define ULTRASOUND_SAMPLE_SIZE 3

// Amount to delay after movement
#define DELAY_AFTER_MOVEMENT 100

// Serial device characteristics, speed and amount to delay after communicating
// over serial port
#define SERIAL_SPEED 9600
#define DELAY_FOR_SERIAL_COMM 8

// Amount to delay after serial device is initialized, this gives you time
// to connect monitor/program
#define DELAY_AFTER_SERIAL_STARTUP 10000

// This is the world dimensions... due to memory limitations we assume it's
// rectangular
float worldXDimension;
float worldYDimension;

#endif