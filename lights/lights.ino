#include <Sparki.h>
#include "sparkiCommon.h"
#include "ultrasonicCommon.h"
#include "determineWorld.h"
#include "lights.h"

byte counter;

void setup() {
  // put your setup code here, to run once:
  #if USE_LCD 
    sparki.clearLCD();
    delay(500);
  #else
    Serial.begin(SERIAL_SPEED);
    sparki.beep();
    delay(DELAY_AFTER_SERIAL_STARTUP);  
    sparki.beep();
  #endif
  counter = 0;
}

void loop() {
  // Just want to test once :)
  if (counter == 0) {
    // Make sure the servo isn't blocking a light
    positionServo(0);
  
    // Calculate world
    calculateRectangularCoordinates();
    
    sampleWorldLights();
    sparki.beep();
    delay(2000);

    if (true == true) {
      setPotentialLightTargets();
    }
    
    if (true == false) {
    
      // Test this it aint working as expected
      calculateLightDeltas();
      int theAngle = getAngleWithHighestLightDelta(-1,-1);  // Use invalid angle to not ignore
      showLightDirection(theAngle);

      // Get the next brightest
      theAngle = getAngleWithHighestLightDelta(getAngle(theAngle-90),getAngle(theAngle+90));
      showLightDirection(theAngle);

      
    }
    counter++;
  }
}
