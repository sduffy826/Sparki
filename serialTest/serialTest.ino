#include <Sparki.h>  // include the sparki library
 
void setup()
{
}
 
void loop()
{
  if (Serial.available()) 
  {
    // int inByte = Serial.read();
    // Serial.print((char)inByte); 
    Serial.println("Hi mom");
    delay(8); // Number of milliseconds to delay
  }
}
