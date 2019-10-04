/*******************************************
 Basic Ultrasonic test
 
 Show the distance Sparki's eyes are reading
 on the LCD. Sparki will beep when something
 is too close. If it measures -1, that means 
 the sensor is either too close or too far 
 from an object
********************************************/
#include <Sparki.h> // include the robot library

unsigned long startTime;
unsigned long deltaTime; 

int angleIncrement, angleDirection, currentAngle, minAngle, maxAngle;
void setup() {
  startTime = millis();
  
  minAngle = -90;
  maxAngle = 90;

  angleDirection = 1;
  angleIncrement = 5;
  currentAngle = minAngle;
}
 
void loop() {
    sparki.clearLCD();
    // Calculage angle between -90 and 90
    if (currentAngle <= minAngle) {
      angleDirection = 1;
    }
    else if (currentAngle >= maxAngle ) {
      angleDirection = -1;
    }
    
    unsigned long deltaTime = millis() - startTime;

    sparki.servo(currentAngle);
    
    //delay(1000);
    int cm = sparki.ping(); // measures the distance with Sparki's eyes
 
    sparki.print("Distance: "); 
    sparki.print(cm); // tells the distance to the computer
    sparki.println(" cm"); 
    sparki.println(" ");
    sparki.print("Time millis: ");
    sparki.println(deltaTime);
    sparki.print("Angle: ");
    sparki.println(currentAngle);
 
    if (cm != -1) { // make sure its not too close or too far
        if (cm < 10) {// if the distance measured is less than 10 centimeters
            sparki.beep(); // beep!
        }
    }
 
    sparki.updateLCD();
    delay(100); // wait 0.1 seconds (100 milliseconds)

    currentAngle += ( angleIncrement * angleDirection) ;
    
}
