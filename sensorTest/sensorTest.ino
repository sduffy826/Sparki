#include <Sparki.h>
const bool DEBUGIT = false;

const float cmPerSec = 2.78;  // How fast sparki travels

const byte ACCEL = 0;
const byte BOTT = 1;
const byte LIGHT = 2;
const byte MAGNE = 3;
const byte MOVE = 4;
const byte ULTRA = 5;
const byte NUMSENSORS = 6;

// How many tests to run
const unsigned int runTests = 10000;

const bool STOPAFTERMOVEMENTCOMPLETES = true;

// Max run time in seconds
const int runTimeInSeconds = 6000;
// Max run time in milliseconds
const unsigned long maxRunTimeInMillis = runTimeInSeconds * 1000;

// Delay after writing to the serial port, in milliseconds
const int serialDelay = 10;

// Serial port speed
const unsigned long serialSpeed = 19200;

// Vars for timing
unsigned long startTime;
unsigned long elapsedTime;
unsigned long currMillis;

byte ultrasonicAngle;
int testIterations = 0;
bool amMoving = false;
unsigned int timeInMillisToGetThere;
bool continueTests = true;  // Can set this at various points to stop tests

struct sensors {
  unsigned long lastMillis;
  bool shouldRun;
};
typedef struct sensors Sensors;
Sensors sparkiSensors[6];

// Didn't put this in alphabetical order, since want to tweak frequently
// for different tests.
// Default all sensors to not running, then turn on one's you want
// ------------------------------------------------------------------------
void initIt() {
  for (int i = 0; i < NUMSENSORS; i++) {
    sparkiSensors[i].lastMillis = startTime;
    sparkiSensors[i].shouldRun = false;
  }
  sparkiSensors[ACCEL].shouldRun = true;
  sparkiSensors[BOTT].shouldRun = true;
  sparkiSensors[LIGHT].shouldRun = true;
  sparkiSensors[MAGNE].shouldRun = true;
  sparkiSensors[MOVE].shouldRun = true;
  sparkiSensors[ULTRA].shouldRun = true;
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
void magnetometerTest() {
  currMillis  = millis();
  elapsedTime = currMillis - sparkiSensors[MAGNE].lastMillis;
  sparkiSensors[MAGNE].lastMillis = currMillis;
  
  // magnetometer, x is left/right movement, y is foward/backward and z is up/down
  float x  = sparki.magX();   // measure the accelerometer x-axis
  float y  = sparki.magY();   // measure the accelerometer y-axis
  float z  = sparki.magZ();   // measure the accelerometer z-axis
  Serial.print(elapsedTime);
  Serial.print(",MAGNETOMETER,x,");
  Serial.print(x);
  Serial.print(",y,");
  Serial.print(y);
  Serial.print(",z,");
  Serial.println(z);
  delay(serialDelay);
}

// ------------------------------------------------------------------------
void moveTest(bool forward, int distanceInCM) {
  currMillis  = millis();
  elapsedTime = currMillis - sparkiSensors[MOVE].lastMillis;
  sparkiSensors[MOVE].lastMillis = currMillis;
  
  Serial.print(elapsedTime);
  Serial.print(",MOVE,");
  if (forward)
    Serial.print("forward,distance,");
  else
    Serial.print("backward,distance,");
    
  Serial.print(distanceInCM);
  Serial.println(",cm");
  
  if (forward)
    sparki.moveForward(distanceInCM);
  else
    sparki.moveBackward(distanceInCM);
  //delay(1000);
  //sparki.moveStop();

  // sparki.moveLeft(degrees);
  // sparki.moveRight(degrees);  
  // sparki.moveStop();  // stop it
  delay(serialDelay);
}

// ------------------------------------------------------------------------
void moveTestNonBlocking(bool forward, int distanceInCM) {
  currMillis  = millis();

  // If not currenting moving then calculate time to get to distance,
  // set the lastMillis to be the current time and move :)
  if (amMoving == false) {
    timeInMillisToGetThere = (distanceInCM / cmPerSec) * 1000;
    sparkiSensors[MOVE].lastMillis = currMillis;
    if (forward)
      sparki.moveForward();
    else
      sparki.moveBackward();
    amMoving = true;
  }
  
  elapsedTime = currMillis - sparkiSensors[MOVE].lastMillis;
  Serial.print(elapsedTime);
  Serial.print(",MOVE,");
  if (forward)
    Serial.print("forward,distanceSoFar,");
  else
    Serial.print("backward,distanceSoFar,");
    
  Serial.print((elapsedTime*cmPerSec)/1000.0);
  Serial.println(",cm");
  
  // See if we're done moving... basically the time to get there has elapsed
  if (elapsedTime >= timeInMillisToGetThere) {
    sparki.moveStop();  // stop it  
    amMoving = false;
    if (STOPAFTERMOVEMENTCOMPLETES)
      continueTests = false;
  }
  delay(serialDelay);
}


// ------------------------------------------------------------------------
void setup() {
  // put your setup code here, to run once:
  setUltrasonicAngle(0);
  
  Serial.begin(serialSpeed);
  delay(1000);

  startTime = millis();
  initIt();
}

// ------------------------------------------------------------------------
void setUltrasonicAngle(int theAngle) {
  ultrasonicAngle = theAngle;
  sparki.servo(ultrasonicAngle);
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

// put your main code here, to run repeatedly
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
        sparki.print("running index: ");
        sparki.print(i);
        sparki.println(" is true");
      }
    }
    sparki.updateLCD();
    delay(5000);
  }
  
  if ((testIterations < runTests) && ((millis() - startTime) < maxRunTimeInMillis)  && continueTests) {
    if (testIterations == 0) {
      // Put message on lcd to connect to serial port
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
    
    testIterations++;
    
    if (sparkiSensors[ACCEL].shouldRun) {
      accelTest();
    }

    if (sparkiSensors[BOTT].shouldRun) {
      bottomInfraredTest();
    }

    if (sparkiSensors[LIGHT].shouldRun) {
      lightTest();
    }
    
    if (sparkiSensors[MAGNE].shouldRun) {
      magnetometerTest();
    }
      
    if (sparkiSensors[MOVE].shouldRun) {
      // moveTest(true, 10);  // Forward 10 cm (~4")
      moveTestNonBlocking(true, 43);
    }
   
    if (sparkiSensors[ULTRA].shouldRun) {
      ultrasonicTest();
    }
    
  }
  else {
    Serial.println("DONE");
    delay(10000);  // delay 10 seconds
  }
}
