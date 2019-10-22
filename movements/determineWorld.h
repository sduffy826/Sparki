#ifndef _INCL_WORLD
#define _INCL_WORLD

#include <Sparki.h>
#include "sparkiCommon.h"
#include "localizationCommon.h"
#include "ultrasonicCommon.h"

void adjustWorldCoordinate(float newX, float newY) {
  worldXDimension = newX;
  worldYDimension = newY;
}

float getWorldXDimension() {
  return worldXDimension;
}

float getWorldYDimension() {
  return worldYDimension;
}

void showWorld() {
  // Write to the lcd or the serial device 
  #if USE_LCD 
    sparki.print("World dimensions x: ");
    sparki.print(worldXDimension);
    sparki.print(" y: ");
    sparki.println(worldYDimension);
    
    sparki.print("My position x: ");
    sparki.print(getCurrentXPosition());
    sparki.print(" y: ");
    sparki.println(getCurrentYPosition());
    sparki.updateLCD();
    delay(5000);
  #else
    Serial.print("DW,x,");
    Serial.print(getWorldXDimension());
    Serial.print(",y,");
    Serial.println(getWorldYDimension());

    Serial.print("DP,x,");
    Serial.print(getCurrentXPosition());
    Serial.print(",y,");
    Serial.println(getCurrentYPosition());
    
    delay(DELAY_FOR_SERIAL_COMM);
  #endif
}

// This calculates the rectangular coordinates of the world for this to
// work the sparki must be perpenticular to a wall, could make it smart
// and take readings within a 45' arc and position itself at the minimum
// distance, but that's not really a requiement for this.

// Note the world coordinates must be within the ultrasounds range... if it's
// larger than that then the world calculated here will be the range... your
// robot should be smart enough to handle that (i.e. if moving and finds world
// dimensions differ, then make an adjustment).
void calculateRectangularCoordinates() {
  delay(DELAY_AFTER_MOVEMENT);
  
  worldXDimension = distanceAtAngle(0) + ULTRASONIC_FORWARD_OF_CENTER;
  sparki.moveRight(90);
  delay(DELAY_AFTER_MOVEMENT);

  // We save the distance from the right wall as the yPosition
  setCurrentYPosition(distanceAtAngle(0) + ULTRASONIC_FORWARD_OF_CENTER); 
  
  sparki.moveRight(90);
  delay(DELAY_AFTER_MOVEMENT);

  setCurrentXPosition(distanceAtAngle(0) + ULTRASONIC_FORWARD_OF_CENTER);
  worldXDimension = worldXDimension + getCurrentXPosition();
  sparki.moveRight(90);
  delay(DELAY_AFTER_MOVEMENT);
  
  worldYDimension = distanceAtAngle(0) + ULTRASONIC_FORWARD_OF_CENTER + getCurrentYPosition();

  // Go back to starting position
  sparki.moveRight(90);
  showWorld();
  setCurrentAngle(0.0);  // We are back at origin, set angle to 0'
}

#endif
