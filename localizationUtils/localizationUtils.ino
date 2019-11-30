#include <Sparki.h>
#include "localizationCommon.h"

// Little test program to show localization routines, the meat is in the
// include file (localizationCommon.h)

byte counter = 0;

void setup() {
  // put your setup code here, to run once:
  setPose(10.0, 2.0, 15.0);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (counter == 0) {
    // We'll move a distance of 4.9 at a 20' angle
    float newX = calculateNewX(4.9, 20.0);
    float newY = calculateNewY(4.9, 20.0);

    // Set new location
    setPose(newX, newY, calculateRealAngleWithAdjustment(20.0));  
    showLocation();
    delay(5000);
    
    // Show another call, we'll move do it another way
    setPose(20.0, 4.0, 15.0);
    setNewPosition(4.9, 20.0);  
    showLocation();

    // Test it with negative, we're currently at 35 degrees 
    float minAngle = getShortestAngleDeltaToGetToOrientation(315);
    sparki.clearLCD();
    sparki.println("From 35 to 315, min angle is:");
    sparki.println(minAngle);
    sparki.updateLCD();
    setNewPosition(0,minAngle);
    showLocation();

    counter++;
  }
}




