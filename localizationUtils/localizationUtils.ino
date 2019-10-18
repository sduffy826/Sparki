#include <Sparki.h>
#include "localizationCommon.h"

byte counter = 0;

void setup() {
  // put your setup code here, to run once:
  setCurrentLocalizationValues(10.0, 2.0, 15.0);
}



void loop() {
  // put your main code here, to run repeatedly:
  if (counter == 0) {
    // We'll move a distance of 4.9 at a 20' angle
    float newX = calculateNewX(4.9, 20.0);
    float newY = calculateNewY(4.9, 20.0);

    // Set new location
    setCurrentLocalizationValues(newX, newY, calculateRealAngleWithAdjustment(20.0));  
    showLoc();
    delay(5000);
    counter++;

    // Show another call, we'll move do it another way
    setCurrentLocalizationValues(20.0, 4.0, 15.0);
    setNewPosition(4.9, 20.0);  
    showLoc();
    
    
  }
}




