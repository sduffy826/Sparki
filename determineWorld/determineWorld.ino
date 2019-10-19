#include <Sparki.h>
#include "localizationCommon.h"
byte counter = 0;
const byte SAMPLESIZE = 5;
const byte SAMPLEDELAY = 8;
float xDimension = 0.0;
float yDimension = 0.0;

void setup() {
  // put your setup code here, to run once:
  setCurrentLocalizationValues(10.0,2.0,15.0);
}

int getPingLength() {
  // Sample
  long temp = 0;
  for (int i = 0; i < SAMPLESIZE; i++) {
    temp = temp + sparki.ping();
    delay(SAMPLEDELAY);
  }
  return (int)(((float)temp/(float)SAMPLEDELAY)+.5);
}

void showWorld() {
  sparki.clearLCD();
  sparki.print("xDimension: ");
  sparki.print(xDimension);
  sparki.print(" yDimension: ");
  sparki.println(yDimension);
  sparki.updateLCD();
  delay(5000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (counter == 0) {
    // We'll move a distance of 4.9 at a 20' angle
    /*
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
    */
    xDimension = getPingLength();
    
    sparki.moveRight(90);
    yDimension = getPingLength();
    sparki.moveRight(90);
    showWorld();
    counter++;
  }
}
