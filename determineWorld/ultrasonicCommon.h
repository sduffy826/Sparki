#ifndef _INCL_ULTRA
#define _INCL_ULTRA

#include <Sparki.h>
#include "sparkiCommon.h"

int ultrasonicTempInt; // declare once as global to save space
int ultrasonicAngle = 190; // Init to invalid angle

void positionServo(int theAngle) {
  if (ultrasonicAngle != theAngle ) {
    sparki.servo(theAngle + ULTRASONIC_SERVO_ANGLE_ADJUSTMENT);
    
    delay(ULTRASONIC_DELAY_AFTER_SERVO_MOVEMENT);
    if ((theAngle - ultrasonicAngle) < -90 ||
        (theAngle - ultrasonicAngle) > 90) {
      delay(ULTRASONIC_DELAY_AFTER_SERVO_MOVEMENT);
    }
    ultrasonicAngle = theAngle;
  }
}

int distanceAtAngle(int angleOfServo) {
  positionServo(angleOfServo);
  ultrasonicTempInt = 0;
  for (byte i = 0; i < ULTRASONIC_SAMPLE_SIZE; i++) {
    delay(ULTRASONIC_SAMPLE_DELAY);
    ultrasonicTempInt += sparki.ping();
  }
  ultrasonicTempInt = (int)( ((float)ultrasonicTempInt / (float)ULTRASONIC_SAMPLE_SIZE) + .5);
  return ultrasonicTempInt > ULTRASONIC_MAX_RANGE ? ULTRASONIC_MAX_RANGE : ultrasonicTempInt;
}

// To the right is positive degrees and left is negative (not sure why they coded this way)
int distanceRight() {
  return distanceAtAngle(ULTRASONIC_RIGHT_ANGLE);
}

int distanceLeft() {
  return distanceAtAngle(ULTRASONIC_LEFT_ANGLE);
}

// For debugging we may want to show values on lcd screen
void showUltrasonic(int theAngle, int theDistance) {
   #if USE_LCD 
    sparki.clearLCD(); // wipe the LCD clear
    sparki.print("<: ");
    sparki.print(theAngle);
    sparki.print("distance: ");
    sparki.println(theDistance);
    sparki.updateLCD(); // put the drawings on the screen
  #else
    Serial.print("US,<,");
    Serial.print(theAngle);
    Serial.print(",d,");
    Serial.println(theDistance);
    delay(DELAY_FOR_SERIAL_COMM);
  #endif
}
#endif
