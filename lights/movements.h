#ifndef _INCL_MOVE
#define _INCL_MOVE

#include <Sparki.h>
#include "sparkiCommon.h"
#include "localizationCommon.h"
#include "ultrasonicCommon.h"

struct MovementAttributes {
  unsigned int startTime;
  unsigned int amMoving : 1;
  unsigned int reserved : 7; // bits avail within boundry
};

MovementAttributes movementState;

void initMovements() {
  positionServo(0);
  movementState.amMoving  = false;
  movementState.startTime = 0;
}

// Routine to return elapsed time... older version below in case you need it
unsigned int getElapsed() {
  return millis() - movementState.startTime;
}
/* unsigned int getElapsed(bool setTime) { 
  unsigned int temp = millis() - movementState.startTime;
  if (setTime) {
    // if want to set the time increment startTime by the elapsed amount
    // could have saved millis but that'd introduce another variable and want to save memory
    movementState.startTime = temp + movementState.startTime;
  }
  return temp;
} */

// Return number of milliseconds required to cover a distance.
unsigned int getMillisToGetThere(float distanceInCM) {
  return (distanceInCM / VELOCITY_CM_P_SEC) * 1000;
}

// Routine to signify we're moving
void startMovingForward() {
  if ( movementState.amMoving == false ) {
    movementState.amMoving = true;
    movementState.startTime = millis();
    sparki.moveForward();
  }
}

// Return the distance traveled so far
float getDistanceTraveledSoFar() {
  if ( movementState.amMoving == true ) {
    return ((getElapsed() * VELOCITY_CM_P_SEC) / 1000.0);
  }
  return 0.0;
}

// Stop moving and return the distance traveled
void stopMoving() {
  // Set your position, the angle delta is 0 since we're going in a straight line
  setNewPosition(getDistanceTraveledSoFar(), 0.0);
  sparki.moveStop();
  movementState.amMoving = false;
}

// Handles moving forward a given distance, second parm is the min distance you can be within of a wall
// Intent is that this will be called continuously... it doesn't block... it'll start moving if sparki isn't
// and it'll stop when we've reached destination or wall.
boolean moveForward(float distanceToTravel, float minAllowedDistanceToObstacle) {
  if ( movementState.amMoving == true ) {
    if ((getDistanceTraveledSoFar() >= distanceToTravel) || (distanceAtAngle(0) <= minAllowedDistanceToObstacle)) {  // done moving
      stopMoving();
    } 
  }
  else {
    if ((getMillisToGetThere(distanceToTravel) <= 0.0) || (distanceAtAngle(0) <= minAllowedDistanceToObstacle)) {  // cant move
      sparki.beep();
    }
    else {
      startMovingForward();
    }
  } 
  return movementState.amMoving;
}  

// Turn left or right byt certain degrees and set your current orientation angle
void turnLeft(byte degrees) {
  sparki.moveLeft(degrees);
  setCurrentAngle(calculateRealAngleWithAdjustment((float)degrees));
}
  
void turnRight(byte degrees) {
  sparki.moveRight(degrees);
  setCurrentAngle(calculateRealAngleWithAdjustment(-(float)degrees));
}

// -------------------------- 
// Right now it only moves to the left, need to make
// it smart enough to turn right too
void followWall() {
  int saveX, saveY;
  int theDistance;
  byte numTurns;
  // Face the wall to the right
  turnRight(90);
  theDistance = distanceAtAngle(0);
  // Move forward distance to wall, less turn radius, second parm is min distance
  // to stay away from wall.
  while (moveForward(theDistance-ULTRASONIC_MIN_SAFE_DISTANCE, ULTRASONIC_MIN_SAFE_DISTANCE));
  saveX = getCurrentXPositionInCM();
  saveY = getCurrentYPositionInCM();

  // Min turns required to go around a rectangular space is 5, we won't make more than 20 and
  // we will stop when back at the original location
  numTurns = 0;
  while ((numTurns < 5) || 
         (numTurns < 20 &&  
          saveX != getCurrentXPositionInCM() &&
          saveY != getCurrentYPositionInCM())) {
    turnLeft(90);
    numTurns++;
    theDistance = distanceAtAngle(0);
    while (moveForward(theDistance-ULTRASONIC_MIN_SAFE_DISTANCE, ULTRASONIC_MIN_SAFE_DISTANCE)) {
      showLocation();
      delay(100);
    }
  } 
}

float moveForwardTillCloseToObstacle(byte howClose) {

  

  
}

void showTurnRadius() {
  // Write to the lcd or the serial device 
  #if USE_LCD 
    sparki.print("Sparki turn radius: ");
    sparki.println(getTurnRadius());
    sparki.updateLCD();
    delay(5000);
  #else
    Serial.print("MR,");
    Serial.print(TURN_RADIUS);
    delay(DELAY_FOR_SERIAL_COMM);
  #endif
}

void showWallMovements() {
  #if USE_LCD 
    sparki.print("Sparki turn radius: ");
    sparki.println(getTurnRadius());
    sparki.updateLCD();
    delay(5000);
  #else
    Serial.print("MW,");
    // Serial.print(distanceTraveledSoFar());
    delay(DELAY_FOR_SERIAL_COMM);
  #endif
}


#endif
