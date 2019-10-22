#include <Sparki.h>  // include the sparki library
int count = 0;

// boolean data takes a byte, but you can declare a struction and define each unsigned int with a length in bits, 
// you can then store true/false values in there... cool way to save memory
struct EightBits {
  unsigned int bit7 : 1;
  unsigned int bit6 : 1;
  unsigned int bit5 : 1;
  unsigned int bit4 : 1;
  unsigned int bit3 : 1;
  unsigned int bit2 : 1;
  unsigned int bit1 : 1;
  unsigned int bit0 : 1;
};

EightBits bunchaBits[64];
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
    for (int i = 0; i < 64; i++) {
      bunchaBits[i].bit1 = false;
      bunchaBits[i].bit0 = false;
    }
    
    count++;
    if (true == false) {
      float theAngle = 405.10;

      sparki.clearLCD(); // wipe the LCD clear
      sparki.print("Mod of angle: ");
      sparki.print(theAngle);
      sparki.print(" is: ");
      float test = modAngle(theAngle);
      sparki.println(test);
      sparki.updateLCD(); // put the drawings on the screen
    }
    sparki.moveRight(90);
  }
}
  
