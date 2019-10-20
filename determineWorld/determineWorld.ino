#include <Sparki.h>
#include "sparkiCommon.h"
#include "localizationCommon.h"
#include "determineWorld.h"

byte counter = 0;

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
  sparki.servo(ULTRASOUND_SERVO_ANGLE_ADJUSTMENT);
}

void loop() {
  // Just want to test once :)
  if (counter == 0) {
    // routine below has the logic to calculate our rectangular world coordinates
    calculateRectangularCoordinates();
    counter++;
  }
}
