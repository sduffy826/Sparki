#include <Sparki.h>
#include "sparkiCommon.h"

#include "localizationCommon.h"
#include "determineWorld.h"
#include "movements.h"

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
    if (true == true) {
      // routine below has the logic to calculate our rectangular world coordinates
      initMovements();
      followWall();
    }

    if (true == false) {
      showTurnRadius();
    }

    if (true == false) {
      turnRight(90);
    }
    
    counter++;
  }
}
