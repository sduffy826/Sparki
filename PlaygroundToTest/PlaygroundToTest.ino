#include <Sparki.h>  // include the sparki library
int count = 0;

void setup() {
  // put your setup code here, to run once:

}

float modAngle(float theAngle) {
  if (theAngle >= 360.0) 
    return modAngle(theAngle-360.0);
  else
    return theAngle;
}


void loop() {
  // put your main code here, to run repeatedly:
  if (count == 0) {
    count++;
    float theAngle = 405.10;
  

    sparki.clearLCD(); // wipe the LCD clear
    sparki.print("Mod of angle: ");
    sparki.print(theAngle);
    sparki.print(" is: ");
    float test = modAngle(theAngle);
    sparki.println(test);
    sparki.updateLCD(); // put the drawings on the screen
  
  }
}
  
