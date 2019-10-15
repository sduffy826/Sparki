#include <Sparki.h>

const unsigned int numIterations = 2;
unsigned int numLoops = 0;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  numLoops++;
  sparki.print(numLoops);
  if (numLoops <= numIterations) {
    // Move forward 10 cm, then back again
    //sparki.moveForward(10);
    //sparki.moveBackward(5);
    
    // Turn left 90 degrees
    //sparki.moveLeft(90);
    
    
    sparki.println("In loop");
    sparki.updateLCD();

    // Large arc counter clock wise
    sparki.motorRotate(MOTOR_LEFT, DIR_CCW, 70);
    sparki.motorRotate(MOTOR_RIGHT, DIR_CW, 100);
    
    // This is reverse
    //sparki.motorRotate(MOTOR_LEFT, DIR_CW, 10);
    //sparki.motorRotate(MOTOR_RIGHT, DIR_CCW, 10);
    delay(5000);
    //sparki.motorRotate(MOTOR_LEFT, DIR_CW, 0);
    //sparki.motorRotate(MOTOR_RIGHT, DIR_CW, 0);
    //delay(2000);
    // sparki.motorRotate(int motor, int direction, int speed);
    // where motor is MOTOR_LEFT, MOTOR_RIGHT (for wheels), or MOTOR_GRIPPER for gripper.  direction is DIR_CW for clockwise or DIR_CCW for counter clockwise, and speed is a value as percentage, from 0->100.

  }

}
