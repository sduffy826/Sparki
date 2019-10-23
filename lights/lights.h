#ifndef _INCL_LIGHT
#define _INCL_LIGHT

#include <Sparki.h>
#include "sparkiCommon.h"
#include "movements.h"
#include "ultrasonicCommon.h"

#define LIGHTSAMPLEANGLE 30
#define LIGHTSAMPLESIZE 3
#define LIGHTSAMPLEDELAY 50
#define LIGHTSAMPLEVALUE2USE 1  // The median value
#define LIGHTCALIBRATIONARRAYSIZE 360/30  // Degrees / Sample Angle

struct LightAttributes {
  unsigned int lightLeft : 10;
  unsigned int lightCenter : 10;
  unsigned int lightRight : 10;
  unsigned int flag1 : 1;
  unsigned int flag2 : 1;
};
LightAttributes lightCalibration[LIGHTCALIBRATIONARRAYSIZE];  // Must coincide with sample angle
LightAttributes lightSample[4];

// Gets the light attributes, well take the median (middle value), that'll be loaded
// into position 0
void setLightAttributes() {
  for (byte i = 0; i < LIGHTSAMPLESIZE; i++) {
    lightSample[i].lightLeft = sparki.lightLeft();
    lightSample[i].lightCenter = sparki.lightCenter();
    lightSample[i].lightRight = sparki.lightRight();
    delay(LIGHTSAMPLEDELAY);
  }
  // Sort arrays, simple bubble sort
  for (byte i = 0; i < LIGHTSAMPLESIZE-1; i++) {
    for (byte j = i + 1; j < LIGHTSAMPLESIZE; j++) {
      if (lightSample[i].lightLeft > lightSample[j].lightLeft) {
        // Swap values... our array is one larger than needed so use index LIGHTSAMPLESIZE as hold area
        lightSample[LIGHTSAMPLESIZE].lightLeft = lightSample[i].lightLeft;
        lightSample[i].lightLeft = lightSample[j].lightLeft;
        lightSample[j].lightLeft = lightSample[LIGHTSAMPLESIZE].lightLeft;
      }
      if (lightSample[i].lightCenter > lightSample[j].lightCenter) {
        lightSample[LIGHTSAMPLESIZE].lightCenter = lightSample[i].lightCenter;
        lightSample[i].lightCenter = lightSample[j].lightCenter;
        lightSample[j].lightCenter = lightSample[LIGHTSAMPLESIZE].lightCenter;
      }
      if (lightSample[i].lightRight > lightSample[j].lightRight) {
        // Swap values... our array is one larger than needed so use index LIGHTSAMPLESIZE as hold area
        lightSample[LIGHTSAMPLESIZE].lightRight = lightSample[i].lightRight;
        lightSample[i].lightRight = lightSample[j].lightRight;
        lightSample[j].lightRight = lightSample[LIGHTSAMPLESIZE].lightRight;
      }    
    }
  }
  
}

void showLightAttributes(int theAngle) {
  #if USE_LCD 
    sparki.print("Light angle");
    sparki.print(theAngle);
    sparki.print(",lft,");
    sparki.print(lightSample[LIGHTSAMPLEVALUE2USE].lightLeft);
    sparki.print(",cntr,");
    sparki.print(lightSample[LIGHTSAMPLEVALUE2USE].lightCenter);
    sparki.print(",rt,");
    sparki.println(lightSample[LIGHTSAMPLEVALUE2USE].lightRight);
    sparki.updateLCD();
  #else
    Serial.print("LA,<,");
    Serial.print(theAngle);
    Serial.print(",l,");
    Serial.print(lightSample[LIGHTSAMPLEVALUE2USE].lightLeft);
    Serial.print(",c,");
    Serial.print(lightSample[LIGHTSAMPLEVALUE2USE].lightCenter);
    Serial.print(",r,");
    Serial.println(lightSample[LIGHTSAMPLEVALUE2USE].lightRight);
    delay(DELAY_FOR_SERIAL_COMM);
  #endif  
};

void sampleWorld() {
  if (getCurrentAngle() != 0.0) {
    turnRight(getCurrentAngle());
  }
  for (int i = 0; i < LIGHTCALIBRATIONARRAYSIZE; i++) {
    setLightAttributes();

    lightCalibration[i].lightLeft = lightSample[LIGHTSAMPLEVALUE2USE].lightLeft;
    lightCalibration[i].lightCenter = lightSample[LIGHTSAMPLEVALUE2USE].lightCenter;
    lightCalibration[i].lightRight = lightSample[LIGHTSAMPLEVALUE2USE].lightRight;
    
    showLightAttributes(i*LIGHTSAMPLEANGLE);
 
    turnLeft(LIGHTSAMPLEANGLE);
    delay(DELAY_AFTER_MOVEMENT);
  }
}

#endif
