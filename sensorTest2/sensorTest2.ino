#include <Sparki.h>
const bool DEBUGIT = true;

const byte ACCEL = 0;
const byte BOTT = 1;
const byte LIGHT = 2;
const byte ULTRA = 5;
const int NUMSENSORS = 6;

// How many tests to run
const long runTests = 100000;
// Max run time in milliseconds
unsigned long maxRunTimeInMillis = 180000;

// Delay after writing to the serial port
const int serialDelay = 10;

// Serial port speed
const unsigned long serialSpeed = 19200;

// Vars for timing
unsigned long startTime;
unsigned long elapsedTime;
unsigned long currMillis;

int ultrasonicAngle;
int testIterations = 0;

struct sensors {
  unsigned long lastMillis;
  bool shouldRun;
};
typedef struct sensors Sensors;
Sensors sparkiSensors[6];

// Default all sensors to not running, then turn on one's you want
void initIt() {
  for (int i = 0; i < NUMSENSORS; i++) {
    sparkiSensors[i].lastMillis = startTime;
    sparkiSensors[i].shouldRun = false;
  }
  sparkiSensors[ACCEL].shouldRun = true;
  sparkiSensors[LIGHT].shouldRun = true;
}

void setup() {
  // put your setup code here, to run once:
  setUltrasonicAngle(0);
  
  Serial.begin(serialSpeed);
  delay(1000);

  startTime = millis();
  initIt();
}

void setUltrasonicAngle(int theAngle) {
  ultrasonicAngle = theAngle;
  sparki.servo(ultrasonicAngle);
}

// ------------------------------------------------------------------------
void accelTest() {
  currMillis  = millis();
  elapsedTime = currMillis - sparkiSensors[ACCEL].lastMillis;
  sparkiSensors[ACCEL].lastMillis = currMillis;
  
  // The z axis is -9.8 at rest, it's m/sec squared (gravity)
  float x  = sparki.accelX();   // measure the accelerometer x-axis
  float y  = sparki.accelY();   // measure the accelerometer y-axis
  float z  = sparki.accelZ();   // measure the accelerometer z-axis
  Serial.print(elapsedTime);
  Serial.print(",ACCELEROMETER,x,");
  Serial.print(x);
  Serial.print(",y,");
  Serial.print(y);
  Serial.print(",z,");
  Serial.println(z);
  delay(serialDelay);
}

// ------------------------------------------------------------------------
void bottomInfraredTest() {
  currMillis  = millis();
  elapsedTime = currMillis - sparkiSensors[BOTT].lastMillis;
  sparkiSensors[BOTT].lastMillis = currMillis;

  // Note 1000 is bright white, 400 is dark, 200 or below is off the edge.
  int edgeLeft   = sparki.edgeLeft();   // measure the left edge IR sensor
  int lineLeft   = sparki.lineLeft();   // measure the left IR sensor
  int lineCenter = sparki.lineCenter(); // measure the center IR sensor
  int lineRight  = sparki.lineRight();  // measure the right IR sensor
  int edgeRight  = sparki.edgeRight();  // measure the right edge IR sensor

  Serial.print(elapsedTime);
  Serial.print(",BOTTOMINFRARED,edgeLeft,");
  Serial.print(edgeLeft);
  Serial.print(",lineLeft,");
  Serial.print(lineLeft);
  Serial.print(",lineCenter,");
  Serial.print(lineCenter); 
  Serial.print(",lineRight,");
  Serial.print(lineRight); 
  Serial.print(",edgeRight,");
  Serial.println(edgeRight);
  delay(serialDelay);
}

// ------------------------------------------------------------------------
void lightTest() {
  currMillis  = millis();
  elapsedTime = currMillis - sparkiSensors[LIGHT].lastMillis;
  sparkiSensors[LIGHT].lastMillis = currMillis;
 
  // Each sensor is slightly different, the higher the number the
  // brighter the light
  int lightLeft   = sparki.lightLeft();
  int lightCenter = sparki.lightCenter();
  int lightRight  = sparki.lightRight();
  Serial.print(elapsedTime);
  Serial.print(",LIGHT,lightLeft,");
  Serial.print(lightLeft);
  Serial.print(",lightCneter,");
  Serial.print(lightCenter);
  Serial.print(",lightRight,");
  Serial.println(lightRight);
  delay(serialDelay);
}

// ------------------------------------------------------------------------
void ultrasonicTest() {
  currMillis  = millis();
  elapsedTime = currMillis - sparkiSensors[ULTRA].lastMillis;
  sparkiSensors[ULTRA].lastMillis = currMillis;
  
  int cm = sparki.ping(); // measures the distance with Sparki's eyes in cm
  Serial.print(elapsedTime);
  Serial.print(",ULTRASONIC,cm,");
  Serial.print(cm);
  Serial.print(",angle,");
  Serial.println(ultrasonicAngle);
  delay(serialDelay);

  //sparki.print("ultrasonicTest: ");
  //sparki.print(cm);
  //sparki.println(" cm");
  //sparki.updateLCD();
}

// ------------------------------------------------------------------------
void loop() {
  if (DEBUGIT) {
    sparki.clearLCD();
    sparki.print("testIterations: ");
    sparki.println(testIterations);
    sparki.print("runTests: ");
    sparki.println(runTests);
    for (int i = 0; i < NUMSENSORS; i++) {
      if (sparkiSensors[i].shouldRun) {
        sparki.print("running index");
        sparki.print(i);
        sparki.println("is true");
      }
    }
    sparki.updateLCD();
    delay(5000);
  }
  
  if ((testIterations < runTests) && ((millis() - startTime) < maxRunTimeInMillis)) {
    if (testIterations == 0) {
      sparki.beep();
      sparki.clearLCD();
      sparki.println("Connect program in 5 seconds");
      sparki.updateLCD();
      delay(5000);
      sparki.clearLCD();
      sparki.println("Started");
      sparki.updateLCD();
      sparki.beep();
    }
    
    //sparki.moveForward(3);
    //delay(10000);
    
    testIterations++;
    // put your main code here, to run repeatedly:
    if (sparkiSensors[ACCEL].shouldRun) {
      sparki.print(ACCEL);
      sparki.println("Calling ACCEL");
      
      accelTest();
    }

    if (sparkiSensors[BOTT].shouldRun) {
      sparki.print(BOTT);
      sparki.println("Calling BOTT");
      bottomInfraredTest();
    }

    if (sparkiSensors[LIGHT].shouldRun) {
      sparki.print(LIGHT);
      sparki.println("Calling LIGHT");
      lightTest();
    }
   
    if (sparkiSensors[ULTRA].shouldRun) {
      sparki.print(ULTRA);
      sparki.println("Calling ULTRA");
      ultrasonicTest();
    }
    sparki.updateLCD();
    delay(3000);
    
  }
//  Serial.print("In main loop. ");
//  Serial.print("Start time: ");
//  Serial.print(startTime);
//  Serial.print(", ultraAngle is: ");
//  Serial.print(ultrasonicAngle);
//  Serial.print(", testIterations is: ");
//  Serial.print(testIterations);
//  Serial.println(". ");
//  delay(8);
  
  //sparki.print("loop: ");
  //sparki.println(testIterations);
  //sparki.updateLCD();
}
