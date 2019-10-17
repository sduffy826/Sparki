#include <Sparki.h>

const unsigned int numIterations = 1;

const float cmPerSec = 2.78;

unsigned int numLoops = 0;
unsigned int startTime;
unsigned int elapsed;
unsigned int currMillis;


void setup() {
  // put your setup code here, to run once:
  startTime = millis();
  currMillis = startTime;
  Serial.begin(19200);
}

unsigned int getElapsed(bool setCurrMillis) {  
  unsigned int temp = millis();
  elapsed = temp - currMillis;
  if (setCurrMillis) {
    currMillis = temp;    
  }
  return elapsed;
}

void moveInArc() {
  // sparki.motorRotate(int motor, int direction, int speed);
  // where motor is MOTOR_LEFT, MOTOR_RIGHT (for wheels), or MOTOR_GRIPPER for gripper.  direction is DIR_CW for clockwise or DIR_CCW for counter clockwise, and speed is a value as percentage, from 0->100.

  // Large arc counter clock wise
  Serial.print(getElapsed(true));
  Serial.println("Starting moveInArc");
  sparki.motorRotate(MOTOR_LEFT, DIR_CCW, 70);
  sparki.motorRotate(MOTOR_RIGHT, DIR_CW, 100);
  Serial.print(getElapsed(true));
  Serial.println("Leaving moveInArc");
}

void moveForward(byte distanceInCM) {
  Serial.print(getElapsed(true));
  Serial.println(" Starting moveForward");
  if (distanceInCM > 0) {
    sparki.moveForward(distanceInCM);
  }
  else {
    sparki.moveForward();
  }
  Serial.print(getElapsed(true));
  Serial.println(" Leaving moveForward");  
}

void moveForwardBasedOnTime(float distanceInCM) {
  unsigned int timeInMillisToGetThere = (distanceInCM / cmPerSec) * 1000;
  
  Serial.print(getElapsed(true));
  Serial.println(" Starting moveForwardBasedOnTime, distance: ");
  Serial.print(distanceInCM);
  Serial.print(" expected to take: ");
  Serial.print(timeInMillisToGetThere);
  Serial.println(" millis");
  sparki.moveForward();
  while (getElapsed(false) < timeInMillisToGetThere) {
    // Serial.print("Waiting for time to expire");
  }
  moveStop();
  Serial.print(getElapsed(true));
  Serial.println(" Leaving moveForwardBasedOnTime");  
}

float moveForwardTillCloseToObstacle(byte howClose) {

  

  
}









void moveBackAndForth(byte distanceInCM) {
  Serial.print(getElapsed(true));
  Serial.print("Starting moveBackAndForth, distance");
  Serial.println(distanceInCM);

  sparki.moveForward(distanceInCM);
  sparki.moveBackward(distanceInCM);
  Serial.print(getElapsed(true));
  Serial.println("Leaving moveBackAndForth"); 
}

void turnLeft(byte degrees) {
  Serial.print(getElapsed(true));
  Serial.print("Starting turnLeft, degrees");
  Serial.println(degrees);
  sparki.moveLeft(degrees);
  Serial.print(getElapsed(true));
  Serial.println("Leaving turnLeft");
}

void turnRight(byte degrees) {
  Serial.print(getElapsed(true));
  Serial.print("Starting turnRight, degrees");
  Serial.println(degrees);
  sparki.moveRight(degrees);
  Serial.print(getElapsed(true));
  Serial.println("Leaving turnRight");
}

void moveStop() {
  sparki.moveStop();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (numLoops == 0) {
    sparki.beep();
    sparki.clearLCD();
    sparki.println("Turn on the serial monitor NOW");
    sparki.updateLCD();
    delay(5000);
    sparki.clearLCD();
    sparki.println("Started");
    sparki.updateLCD();
    sparki.beep();
  }

 
  if (numLoops < numIterations) {
    numLoops++;
    Serial.print("In loop(), loop: ");
    Serial.println(numLoops);

    // Arc test
    if (false == true) {
      moveInArc();
      delay(5000);
      moveStop();
    }

    // Move forward
    if (false == true) {
      moveForward(51);
      delay(5000);
      moveStop();
    }

    if (true == true) {
      moveForwardBasedOnTime(25.4);  // 10 inches
    }

    // Move forward and backward N cm
    if (false == true) {
      moveBackAndForth(20);
      delay(5000);
      moveStop();
    }

    if (false == true) {
      turnLeft(90);
      turnRight(90);
      delay(5000);
      moveStop();
    }
  }
}
