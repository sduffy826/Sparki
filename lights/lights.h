#ifndef _INCL_LIGHT
#define _INCL_LIGHT

#include <Sparki.h>
#include "sparkiCommon.h"
#include "movements.h"
#include "ultrasonicCommon.h"

#define DEBUGLIGHTS false

#define LIGHTSAMPLEANGLE 30
#define LIGHTSAMPLESIZE 3
#define LIGHTSAMPLEDELAY 50
#define LIGHTSAMPLEVALUE2USE 1  // The median value
#define LIGHTCALIBRATIONARRAYSIZE (360/30)  // Degrees / Sample Angle
#define LIGHTANGLERANGETOIGNORE 90 // Degrees to ignore of an existing light source, this is ignore left and right (so it's double)
#define LIGHTSINWORKSPACE 2 // Can't be more than 2 for now
#define LIGHTMINTRIANGULATION 6 // Min cm to be able to triangulate angle
#define LIGHTOPTIMALTRIANGULATION 10  // CM for best triangulation

struct LightAttributes {
  unsigned int lightLeft : 10;
  unsigned int lightCenter : 10;
  unsigned int lightRight : 10;
  unsigned int flag1 : 1;
  unsigned int flag2 : 1;
};

// Struct to hold the pct change 0-100
struct LightDeltaPct {
  unsigned int leftPct : 7;
  unsigned int centerPct : 7;
  unsigned int rightPct : 7;
  unsigned int leftPos : 1;
  unsigned int centerPos : 1;
  unsigned int rightPos : 1;
};

LightAttributes lightCalibration[LIGHTCALIBRATIONARRAYSIZE];  // Must coincide with sample angle
LightDeltaPct lightDeltaPcts[LIGHTCALIBRATIONARRAYSIZE];     // Readings

LightAttributes lightSample[4];

// Gets the light attributes, we take samples and will sort them at the end, the program will use
// a constant (LIGHTSAMPLEVALUE2USE) to determine the value to use (median at time of this writing).
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
  // Note if want average you could put that in the last position... but don't think we need
  
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
}

void sampleWorldLights() {
  turnToZero();
  for (int i = 0; i < LIGHTCALIBRATIONARRAYSIZE; i++) {
    setLightAttributes();

    lightCalibration[i].lightLeft = lightSample[LIGHTSAMPLEVALUE2USE].lightLeft;
    lightCalibration[i].lightCenter = lightSample[LIGHTSAMPLEVALUE2USE].lightCenter;
    lightCalibration[i].lightRight = lightSample[LIGHTSAMPLEVALUE2USE].lightRight;
    lightCalibration[i].flag1 = false;
    lightCalibration[i].flag2 = false;
    showLightAttributes(i*LIGHTSAMPLEANGLE);
 
    turnLeft(LIGHTSAMPLEANGLE);
    delay(DELAY_AFTER_MOVEMENT);
  }
}

// Return the delta pct between current value and calibration one, note it can be negative
int getDeltaPct(int currentValue, int calibrationValue) {
  int deltaAmt = currentValue - calibrationValue; // Delta
  if (deltaAmt > calibrationValue) {
    // Delta is greater than base, return 100
    return 100;
  }
  else
    return (deltaAmt * 100)/calibrationValue;
}

void calculateLightDeltas() {
  int deltaAmt, holdAngle;
  holdAngle = getCurrentAngle();
  turnToZero();
  for (int i = 0; i < LIGHTCALIBRATIONARRAYSIZE; i++) {
    setLightAttributes();

    deltaAmt = getDeltaPct(lightSample[LIGHTSAMPLEVALUE2USE].lightLeft, lightCalibration[i].lightLeft);
    lightDeltaPcts[i].leftPct = (deltaAmt < 0 ? -deltaAmt : deltaAmt);
    lightDeltaPcts[i].leftPos = (deltaAmt < 0 ? false : true); // Positive values have true

    deltaAmt = getDeltaPct(lightSample[LIGHTSAMPLEVALUE2USE].lightCenter, lightCalibration[i].lightCenter);
    lightDeltaPcts[i].centerPct = (deltaAmt < 0 ? -deltaAmt : deltaAmt);
    lightDeltaPcts[i].centerPos = (deltaAmt < 0 ? false : true); // Positive values have true
    
    deltaAmt = getDeltaPct(lightSample[LIGHTSAMPLEVALUE2USE].lightRight, lightCalibration[i].lightRight);
    lightDeltaPcts[i].rightPct = (deltaAmt < 0 ? -deltaAmt : deltaAmt);
    lightDeltaPcts[i].rightPos  = (deltaAmt < 0 ? false : true); // Positive values have true
  
    turnLeft(LIGHTSAMPLEANGLE);
    delay(DELAY_AFTER_MOVEMENT);
  }
  turnToAngle(holdAngle);
}

// Little help routine, just returns an integer representing the delta pct (with the correct sign)
int deltaPctHelper(int deltaPct, bool isPositive) {
  return (isPositive ? deltaPct : -deltaPct);
}

// Helper method
bool numberBetweenRange(int theNum, int lowValue, int highValue) {
  if (lowValue > highValue) {   // It wraps around i.e 11 -> 1
     return ((theNum >= highValue || theNum <= lowValue) ? true : false);
  }
  else {
    return ((theNum >= lowValue && theNum <= highValue) ? true : false);
  }
}

// Return angle with highest brightness (or lowest) depending on arg; it will also a particular
// angle and some delta around it (CONSTANT)
// The multFactor passed in should be 1 for highest and -1 for lowest delta, we also have args to ignore
// a range of angles... idea to ignore range that has a light we're not interested in
int getAngleWithBiggestLightDelta(int multFactor, int angle2IgnoreStart, int angle2IgnoreEnd) {
  // We'll use the center light for now... may want to change but since we're rotating that should
  // be brightest when pointing directly at light
  if (angle2IgnoreStart >= 0) {
    #if DEBUGLIGHTS
      Serial.println("");
      Serial.print("LBIR,<,");
      Serial.print(angle2IgnoreStart);
      Serial.print(",to,");
      Serial.println(angle2IgnoreEnd);
      delay(DELAY_FOR_SERIAL_COMM);
    #endif
  }
 
  int indexPos  = -1;
  int holdValue = -100 * multFactor; // Using a +1 or -1 mult factor will return largest + or largest - #

  for (int i = 0; i < LIGHTCALIBRATIONARRAYSIZE; i++) {
    if (numberBetweenRange(i * LIGHTSAMPLEANGLE, angle2IgnoreStart, angle2IgnoreEnd) == false) {
      if ((indexPos == -1) || 
          ((deltaPctHelper(lightDeltaPcts[i].centerPct, lightDeltaPcts[i].centerPos) * multFactor) > holdValue)) {
        holdValue = deltaPctHelper(lightDeltaPcts[i].centerPct, lightDeltaPcts[i].centerPos) * multFactor;
        indexPos = i;
      }
    }
    else {
      #if DEBUGLIGHTS
        Serial.print("LBI,<,");
        Serial.println(i*LIGHTSAMPLEANGLE);
        delay(DELAY_FOR_SERIAL_COMM);
      #endif
    }
  }
  return indexPos*LIGHTSAMPLEANGLE;
}

// Helper method, will return highest delta, if don't want to ignore angle range pass in -1 and -1 
int getAngleWithHighestLightDelta(int angle2IgnoreStart, int angle2IgnoreEnd) {
  return getAngleWithBiggestLightDelta(1, angle2IgnoreStart, angle2IgnoreEnd);
}

// Helper method, will return smallest delta (good if want to find shadows)
int getAngleWithLowestLightDelta(int angle2IgnoreStart, int angle2IgnoreEnd) {
  return getAngleWithBiggestLightDelta(-1, angle2IgnoreStart, angle2IgnoreEnd);
}

void showLightDirection(int theAngle) {
  int indexOfDelta = theAngle/LIGHTSAMPLEANGLE;
  #if USE_LCD 
    sparki.print("Bright angle");
    sparki.print(theAngle);
    sparki.print(",lft,");
    sparki.print(lightDeltaPcst[indexOfDelta].leftPct);
    sparki.print(",cntr,");
    sparki.print(lightDeltaPcts[indexOfDelta].centerPct);
    sparki.print(",rt,");
    sparki.println(lightDeltaPcts[indexOfDelta].rightPct);
    sparki.updateLCD();
  #else
    Serial.print("LB,<,");
    Serial.print(theAngle);
    Serial.print(",l,");
    Serial.print(deltaPctHelper(lightDeltaPcts[indexOfDelta].leftPct,  lightDeltaPcts[indexOfDelta].leftPos));
    Serial.print(",c,");
    Serial.print(deltaPctHelper(lightDeltaPcts[indexOfDelta].centerPct,  lightDeltaPcts[indexOfDelta].centerPos));
    Serial.print(",r,");
    Serial.println(deltaPctHelper(lightDeltaPcts[indexOfDelta].rightPct,  lightDeltaPcts[indexOfDelta].rightPos));
    delay(DELAY_FOR_SERIAL_COMM);
    
    #if DEBUGLIGHTS
      for (int i = 0; i < LIGHTCALIBRATIONARRAYSIZE; i++) {
        int quadrantOfAngle = getQuadrantAngleIsIn(i*LIGHTSAMPLEANGLE);
        Serial.print("LBD,<,");
        Serial.print(i*LIGHTSAMPLEANGLE);
        Serial.print(",l,");
        Serial.print(deltaPctHelper(lightDeltaPcts[i].leftPct,  lightDeltaPcts[i].leftPos));
        Serial.print(",c,");
        Serial.print(deltaPctHelper(lightDeltaPcts[i].centerPct, lightDeltaPcts[i].centerPos));
        Serial.print(",r,");
        Serial.println(deltaPctHelper(lightDeltaPcts[i].rightPct, lightDeltaPcts[i].rightPos));
        delay(DELAY_FOR_SERIAL_COMM);
      }     
    #endif
  #endif  
};

void setPotentialLightTargets() {
  // routine below has the logic to calculate our rectangular world coordinates
  Pose lights2Review[4];  // 0,1 is for first light, 2,3 is for second  
  lights2Review[0] = getPose();
  lights2Review[2] = getPose();

  // Test this it aint working as expected
  calculateLightDeltas();
  lights2Review[0].angle = getAngleWithHighestLightDelta(-1,-1);  // Use invalid angle to not ignore
  showLightDirection(lights2Review[0].angle);

  if (LIGHTSINWORKSPACE > 1) {
    // Get the next brightest, pass in the angle from the first one... we want to ignore that
    lights2Review[2].angle = getAngleWithHighestLightDelta(getAngle(lights2Review[0].angle-LIGHTANGLERANGETOIGNORE),
                                                           getAngle(lights2Review[0].angle+LIGHTANGLERANGETOIGNORE));
    showLightDirection(lights2Review[2].angle);
  }
  else {
    // We'll use 180 apart
    lights2Review[2].angle = getAngle(lights2Review[1].angle + 180.0);
  }
  // We are going to try and triangulate the position of the lights
  // I'm reusing positions 1 and 3 as work area's... these values won't persist
  // Calculate the angle between the two angles we have, we'll move there to try and triangulate.
  lights2Review[1].angle = getMidpointBetweenTwoAngles(lights2Review[0].angle, lights2Review[2].angle);
  lights2Review[3].angle = getAngle(lights2Review[1].angle + 180); // and calculate 180' away, may need
  
  lights2Review[1].xPos = getDistanceAtAngle(lights2Review[1].angle);
  if (lights2Review[1].xPos < LIGHTMINTRIANGULATION) {
    // It's less than what we want to triangulate, try 180' away
    lights2Review[3].yPos = getDistanceAtAngle(lights2Review[3].angle);
    if (lights2Review[3].xPos < lights2Review[1].xPos) {
      // Go back to original, it's less than we want but more than at 180'
      turnToAngle(lights2Review[1].angle);
    }
  }

  // Move forward
  while (moveForward(LIGHTOPTIMALTRIANGULATION, ULTRASONIC_MIN_SAFE_DISTANCE));

  // Calculate the delta's at a distance
  calculateLightDeltas();
  lights2Review[1].angle = getAngleWithHighestLightDelta(getAngle(lights2Review[2].angle-LIGHTANGLERANGETOIGNORE),
                                                         getAngle(lights2Review[2].angle+LIGHTANGLERANGETOIGNORE));
    
  showLightDirection(lights2Review[1].angle);

  if (LIGHTSINWORKSPACE > 1) {
    // Get the next brightest, pass in the angle from the first one... we want to ignore that
    lights2Review[3].angle = getAngleWithHighestLightDelta(getAngle(lights2Review[0].angle-LIGHTANGLERANGETOIGNORE),
                                                           getAngle(lights2Review[0].angle+LIGHTANGLERANGETOIGNORE));
    showLightDirection(lights2Review[3].angle);
  }

  // Calculate the triangulation
 
}
      




#endif
