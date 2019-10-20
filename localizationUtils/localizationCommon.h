#ifndef _INCL_LOCAL
#define _INCL_LOCAL

#include <Sparki.h>
#include "sparkiCommon.h"

float lastXPosition;
float lastYPosition;
float lastAngleOfOrientation;

void setCurrentLocalizationValues(float x, float y, float angle) {
  lastXPosition = x;
  lastYPosition = y;
  lastAngleOfOrientation = angle;
}

void setCurrentAngle(float angle) {
  lastAngleOfOrientation = angle;
}

float getCurrentAngle() {
  return lastAngleOfOrientation;
}

void setCurrentXPosition(float x) {
  lastXPosition = x;
}

float getCurrentXPosition() {
  return lastXPosition;
}

void setCurrentYPosition(float y) {
  lastYPosition = y;
}

float getCurrentYPosition() {
  return lastYPosition;
}

float degreesToRadians(float degrees) {
  return (degrees * (PI / 180.0));
}

// This is basically mod(angle,360.0), couldn't do it with % and float args so wrote my own
float getAngle(float angleInDegrees) {
  if (angleInDegrees >= 360.0)
    return getAngle(angleInDegrees - 360.0);
  else
    if (angleInDegrees < 0.0) 
      return getAngle(angleInDegrees + 360.0);
    else
      return angleInDegrees;
}

// Common function to return our 'real angle'; it's the angleDelta + prior orientation (it's degrees)
float calculateRealAngleWithAdjustment(float angleDelta) {
  return getAngle(angleDelta + lastAngleOfOrientation);
}

float calculateNewX(float distance, float angleDelta) {
  // Angle delta is the angle relative to the last position, the real angle is that
  // value plus the lastAngleOfOrientation (the angle it was at when we started).
  // remember cos is adjacent/hypotenuse
  // xValue is calculated by 
  // .   calculate the x alone (it's cos(angle)*distanceTraveled
  //     add the x alone to the lastRobot x position
  return ((cos(degreesToRadians(calculateRealAngleWithAdjustment(angleDelta))) * distance) + lastXPosition);
}

float calculateNewY(float distance, float angleDelta) {
  // Similar to the logic for x, but we use sin here (remember sin is Opposite/Hypotenuse)
  // Angle delta is the angle relative to the last position, the real angle is that
  // value plus the currentAngle (the angle it was at when we started).
  return ((sin(degreesToRadians(calculateRealAngleWithAdjustment(angleDelta))) * distance) + lastYPosition);
}

void setNewPosition(float distance, float angleDelta) {
  float newX = calculateNewX(distance, angleDelta);
  float newY = calculateNewY(distance, angleDelta);
  // Set new location
  setCurrentLocalizationValues(newX, newY, calculateRealAngleWithAdjustment(angleDelta));
}

float getShortestAngleDeltaToGetToOrientation(float targetOrientation) {
  // This returns the shortest angle to get from currentAngleOrientation to a target orientation
  // The value returned will be + for normal rotation (left/ccw), it'll be negative for 
  // right/ccw rotation.

  // Note I didn't save the delta because I'm trying to preserver memory

  // The delta of targetOrientation - currentAngle is the degrees moving in right rotation (cw)
  if (targetOrientation - getCurrentAngle() < -180.0) {
    // if less than -180.0 then add 360' to it, this will be + angles (moving ccw)
    return (360.0 + (targetOrientation - getCurrentAngle()));
  }
  else if (targetOrientation - getCurrentAngle() > 180.0) {
    // if more than 180' then subtract 360 from it, we'll move in negative direction (cw)
    return -(360 - (targetOrientation - getCurrentAngle()));
  }
  else {
    return (targetOrientation - getCurrentAngle());
  }
}
 
// For debugging we may want to show values on lcd screen
void showLocation() {
   #if USE_LCD 
    sparki.clearLCD(); // wipe the LCD clear
    sparki.print("x: ");
    sparki.print(lastXPosition);
    sparki.print(" y: ");
    sparki.print(lastYPosition);
    sparki.print(" angle: ");
    sparki.println(lastAngleOfOrientation);
    sparki.updateLCD(); // put the drawings on the screen
  #else
    Serial.print("LO,x,");
    Serial.print(lastXPosition);
    Serial.print(",y,");
    Serial.print(lastYPosition);
    Serial.print(",<,");
    Serial.println(lastAngleOfOrientation);
    delay(DELAY_FOR_SERIAL_COMM);
  #endif
}

#endif


