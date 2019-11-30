#include <Sparki.h>  // include the sparki library

#include "localizationCommon.h"
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

// Struct to hold the pct change 0-100
struct LightDeltaPct {
  unsigned int leftPct : 7;
  unsigned int centerPct : 7;
  unsigned int rightPct : 7;
  unsigned int flag1 : 1;
  unsigned int flag2 : 1;
  unsigned int flag3 : 1;
};

struct testIntStruct {
  unsigned int firstPart : 4;
  unsigned int secondPart : 4;
};


class testIntClass {
  private:
    unsigned int firstPart : 4;
    unsigned int secondPart : 4;
};

class Color {
  private:
    byte rValue;
    byte gValue;
    byte bValue;

  public:
    Color(byte _rValue, byte _gValue, byte _bValue) {
      setRGBValues(_rValue, _gValue, _bValue);
    }
    Color() { }
    
    void setRGBValues(byte _rValue, byte _gValue, byte _bValue) {
      rValue = _rValue;
      gValue = _gValue;
      bValue = _bValue;
    }
    void setRValue(byte _rValue) { rValue = _rValue; }
    byte getRValue() { return rValue; }
    
    void setGValue(byte _gValue) { gValue = _gValue; }
    byte getGValue() { return gValue; }
    
    void setBValue(byte _bValue) { bValue = _bValue; }
    byte getBValue() { return bValue; }
};

LightDeltaPct lightDeltaPct;

EightBits bunchaBits[64];

void testAddrOfColor(const Color &colorArg) {
  Color *aColorPtr;
  aColorPtr = &colorArg;
  sparki.print("rValue: ");
  aColorPtr->setRValue(aColorPtr->getRValue()+10);
  sparki.println(aColorPtr->getRValue());
  
  long int anotherInt = &aColorPtr;
  sparki.print("aColorPtr addr: ");
  sparki.println(anotherInt);

  anotherInt = (int)aColorPtr;
  sparki.print("addrInPtr: ");
  sparki.println(anotherInt);

  sparki.print("sizeOfPtr:");
  sparki.println(sizeof(aColorPtr));
  
}


int testDistance(int distanceAtAngle, int angleOfServo) {
  #define ULTRASONIC_SERVO_TO_PIVOT 2.7
  #define ULTRASONIC_SERVO_PIVOT_OFFSET 1.2

  return ((int)(distanceAtAngle + ULTRASONIC_SERVO_TO_PIVOT + 0.5 + ( ((90 - abs(angleOfServo))/90.0) * ULTRASONIC_SERVO_PIVOT_OFFSET) )) ;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  delay(5000);
  
  
}

int closest90Angle(int theAngle) {
  return ((((theAngle+44)/90)*90)%360);
}
float modAngle(float theAngle) {
  if (theAngle >= 360.0) 
    return modAngle(theAngle-360.0);
  else
    return theAngle;
}

int dumpValues(int a, int b, int c, bool flg, bool flg2, bool flg3) {
 sparki.print("a: ");
 sparki.print(a);
 sparki.print(" b: ");
 sparki.print(b);
 sparki.print(" c: ");
 sparki.println(c);
 sparki.print(" f1: ");
 sparki.print(flg);
 sparki.print(" f2: ");
 sparki.print(flg2);
 sparki.print(" f3: ");
 sparki.println(flg3);
 return ( a > b ? ( a > c ? a : c) : ( b > c ? b : c ));
}

void dumpIt(const byte &a, const byte &b) {
  sparki.clearLCD();
  sparki.print("a:");
  sparki.print(a);
  sparki.print(" sizeof(a): ");
  sparki.println(sizeof(a));
  sparki.print(" b:");
  sparki.println(b);
  sparki.print(" sizeof(b): ");
  sparki.println(sizeof(b));
  sparki.updateLCD();
}

// This works for both
int convertCoordinateSystem(int angle) {
  return (450-angle)%360;
}

// THIS IS WRONG!!! use one above, it works for both
int convertCartesianToCompassCoordinate(int angle) {
  return (450+angle)%360;
}

void printPoints(const int &x1, const int &y1, const int &x2, const int &y2, const bool &showBearing) {
  sparki.print(showBearing ? "B," : "C");
  sparki.print("(");
  sparki.print(x1);
  sparki.print(",");
  sparki.print(y1);
  sparki.print("),(");
  sparki.print(x2);
  sparki.print(",");
  sparki.print(y2);
  sparki.print(") <: ");
  if (showBearing) {
    sparki.println(convertCoordinateSystem(calculateCartesianAngleBetweenPoints(x1,y1,x2,y2)));
  }
  else {
    sparki.println(calculateCartesianAngleBetweenPoints(x1,y1,x2,y2));
  }
}

int calculateCartesianAngleBetweenPoints(const int &x1, const int &y1, const int &x2, const int &y2) {
   //int compassAngle = atan2(x2-x1,y2-y1)*(180/3.14159);
   int compassAngle = atan2(y2-y1,x2-x1)*(180/3.14159);
   //int compassAngle = convertCompassCoordinateToCartesian(atan2(y2-y1,x2-x1)*(180/3.14159));
   return compassAngle;
}

// This is WRONG... the atan2 values are xy, should be y. x  only left for play
int calculateAngle(int x, int y) {
  sparki.clearLCD();

  int x2Use = (x < 0 ? -x : x);
  int compassAngle = 0;

  if (x == 0) {
    if (y < 0) {
      compassAngle = 180;
    }
  }
  else {
    compassAngle = atan2(x,y)*(180/3.14159);
  }
  sparki.print("x: ");
  sparki.println(x);
  sparki.print("y: ");
  sparki.println(y);
  sparki.print("compass<: ");
  
  sparki.println(compassAngle);
  sparki.print("cartesian< ");
  sparki.println(convertCoordinateSystem(compassAngle));
  sparki.print("back to compass: ");
  sparki.println(convertCoordinateSystem(convertCoordinateSystem(compassAngle)));
  //sparki.println(atan2(y/x));
  //sparki.println(atan2(y/x)*(180/3.14159));
  sparki.updateLCD();
}

void checkVariable(int changeMe) {
  changeMe = changeMe + 1;
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
    
    if (true == false) {
      // Show calculation for closest angle  
      for (int theAngle = 0; theAngle < 361; theAngle+=25) {
        sparki.clearLCD();
        sparki.print("<: ");
        sparki.print(theAngle);
        sparki.print(" closest: ");
        sparki.println(closest90Angle(theAngle));
        sparki.updateLCD();
        delay(2000);
      }
    }

    if (true == false) {
      lightDeltaPct.leftPct = 10;
      lightDeltaPct.centerPct = 89;
      lightDeltaPct.rightPct = 30;
      lightDeltaPct.flag1 = true;
      lightDeltaPct.flag2 = false;
      lightDeltaPct.flag3 = true;
      
      sparki.clearLCD();

      int maxValue = dumpValues(lightDeltaPct.leftPct, 
                                lightDeltaPct.centerPct,
                                lightDeltaPct.rightPct,
                                lightDeltaPct.flag1,
                                lightDeltaPct.flag2,
                                lightDeltaPct.flag3);
      sparki.print("Max: " );
      sparki.println(maxValue);
      
      sparki.updateLCD(); 
    }

    if (true == false) {
      #define LIGHTSAMPLEANGLE 30
      #define LIGHTSAMPLESIZE 3
      #define LIGHTSAMPLEDELAY 50
      #define LIGHTSAMPLEVALUE2USE 1  // The median value
      #define LIGHTCALIBRATIONARRAYSIZE (360/30) // Degrees / Sample Angle
      #define LIGHTANGLERANGETOIGNORE 2 // Number of ranges to ignore before and after ignore angle 
      int ignoreAngleStartIdx, ignoreAngleEndIdx;
      int angle2Ignore = 330;
      ignoreAngleStartIdx = ignoreAngleEndIdx = LIGHTCALIBRATIONARRAYSIZE; // an invalid index position

      ignoreAngleStartIdx = ((angle2Ignore/LIGHTSAMPLEANGLE) + (LIGHTCALIBRATIONARRAYSIZE - LIGHTANGLERANGETOIGNORE)) % LIGHTCALIBRATIONARRAYSIZE;  
      ignoreAngleEndIdx   = ((angle2Ignore/LIGHTSAMPLEANGLE) + LIGHTANGLERANGETOIGNORE) % LIGHTCALIBRATIONARRAYSIZE;

      sparki.clearLCD();
      sparki.println(angle2Ignore/LIGHTSAMPLEANGLE);
      sparki.println(LIGHTCALIBRATIONARRAYSIZE - LIGHTANGLERANGETOIGNORE);
      sparki.println(ignoreAngleStartIdx);
      sparki.println((angle2Ignore/LIGHTSAMPLEANGLE) + (LIGHTCALIBRATIONARRAYSIZE - LIGHTANGLERANGETOIGNORE));
      sparki.println(LIGHTCALIBRATIONARRAYSIZE);
      sparki.println(21 % 12);
      sparki.println(21 % LIGHTCALIBRATIONARRAYSIZE);
      sparki.updateLCD(); 
    }


    if (true == false) {
      #define DIVISOR 360
      int someNumber = -90;
      sparki.clearLCD();
      sparki.println(someNumber);
      sparki.println(DIVISOR);
      sparki.println(someNumber % DIVISOR);
      sparki.println(someNumber % (DIVISOR));
      sparki.println(someNumber / DIVISOR);
      sparki.updateLCD();
    }

    // Get getting structure, ensuring it's pass by value
    if (true == false) {
      setPose(5.0, 15.0, 45);
      //showLocation();
      delay(2000);
      Pose aPose;
      aPose = getPose();
      aPose.xPos += 10.0;
      aPose.yPos += 20.0;
      aPose.angle += 30.0;
      delay(2000);
      sparki.clearLCD();
      sparki.println(aPose.xPos);
      sparki.println(aPose.yPos);
      sparki.println(aPose.angle);
      sparki.updateLCD();
      delay(2000);
      showLocation();
    }


    if (true == false) {
      // Remember your pointing toward Y, the compass angle is angle to the right of you as +
      calculateAngle(-3,3);
      delay(2000);
      //delay(2000);
      //calculateAngle(1,1);
    }

    if (true == false) {
      int myValue = 5;
      sparki.clearLCD();
      for (int i = 0; i < 2; i++) {
        checkVariable(myValue);
        sparki.print("myValue: ");
        sparki.println(myValue);
        delay(200);
      }
      sparki.updateLCD();
    }


    if (true == false) {
      // This checks what happens when overflow an unsigned int, also shows size of struct vs class
      testIntStruct myTest;
      
      myTest.firstPart = 15;
      myTest.secondPart = 14;
      for (int i = 0; i < 3; i++) {
        myTest.firstPart++;
        myTest.secondPart++;
        dumpIt(myTest.firstPart, myTest.secondPart);
        delay(2000);
      }

      testIntClass myObj;
      sparki.print("SizeofStruct: ");
      sparki.println(sizeof(myTest));
      sparki.print("SizeofClass: ");
      sparki.println(sizeof(myObj));
      sparki.updateLCD();
      delay(2000);
    }

    if (true == false) {
      sparki.clearLCD();
      sparki.print(abs(-1.3));
      sparki.print(" ");
      sparki.print(abs(1.3));
      sparki.print(" ");
      sparki.println(abs(-3));
      sparki.updateLCD();
    }
     
    if (true == false) {
      sparki.moveRight(90);
    }

    if (true == false) {
      sparki.clearLCD();
      bool showBearingAngle = false;
      if (true == true) {
        printPoints(0,0,5,10,showBearingAngle);
        printPoints(0,0,-5,10,showBearingAngle);
        printPoints(0,0,1,-10,showBearingAngle);
        printPoints(0,0,-1,-10,showBearingAngle);
      }
      if (true == false)  {
        printPoints(0,0,0,1,showBearingAngle);
        printPoints(0,0,1,0,showBearingAngle);
        printPoints(0,0,0,-1,showBearingAngle);
        printPoints(0,0,-1,0,showBearingAngle);
      }
      sparki.updateLCD();
    }

    // Class address testing
    if (true == false) {      
      sparki.clearLCD();
      Color someColor(30,20,10);
      long int longIntVar = &someColor;
      sparki.print("rValue before: ");
      sparki.println(someColor.getRValue());
      
      sparki.print("objAddr: ");
      sparki.println(longIntVar);

      Color *someColorPtr;
      someColorPtr = &someColor;
      sparki.print("someColorPtr: ");
      longIntVar = someColorPtr;
      sparki.println(longIntVar);

      testAddrOfColor(someColor);
      sparki.print("rValue after: ");
      sparki.println(someColor.getRValue());
      
      sparki.updateLCD();
    }

    if (true == false) {
      sparki.clearLCD();
      //#define ULTRASONIC_SERVO_TO_PIVOT 2.7
      ///#define ULTRASONIC_SERVO_PIVOT_OFFSET 1.2

      int distanceAtAngle = 45;
      int angleOfServo = -30;
      sparki.println((90 - abs(angleOfServo))/90.0);
      //sparki.println ((int)(distanceAtAngle + ULTRASONIC_SERVO_TO_PIVOT + 0.5 + ( ((90 - abs(angleOfServo))/90.0) * ULTRASONIC_SERVO_PIVOT_OFFSET) )) ;
      sparki.println(testDistance(distanceAtAngle,angleOfServo));
      sparki.updateLCD();
    }

    // Test struct assignments
    if (true == true) {
      sparki.beep();
      delay(500);
      sparki.beep();
      unsigned long theAddr;
      unsigned long ptrValue;
      testIntStruct *ptrStruct;
      
      testIntStruct struct1;
      struct1.firstPart = 10;
      struct1.secondPart = 6;
      Serial.print("Struct1:");
      Serial.print(struct1.firstPart);
      Serial.print(",");
      Serial.println(struct1.secondPart);
      
      Serial.print("&struct1:");
      theAddr = &struct1;
      ptrStruct = &struct1;
      Serial.print("theAddr:");
      Serial.print(theAddr);
      Serial.print(", ptrStruct:");
      ptrValue = ptrStruct;
      Serial.println(ptrValue);
      
      testIntStruct struct2 = { 2, 4 };      
      Serial.print("Struct2:");
      Serial.print(struct2.firstPart);
      Serial.print(",");
      Serial.println(struct2.secondPart);
     
      Serial.print("&struct2:");
      theAddr = &struct2;
      ptrStruct = &struct2;
      Serial.print("theAddr:");
      Serial.print(theAddr);
      Serial.print(", ptrStruct:");
      ptrValue = ptrStruct;
      Serial.println(ptrValue);


      Serial.println("After assignmentment");
      struct2 = struct1;
     
      Serial.print("Struct2:");
      Serial.print(struct2.firstPart);
      Serial.print(",");
      Serial.println(struct2.secondPart);
       
      Serial.print("&struct2:");
      theAddr = &struct2;
      ptrStruct = &struct2;
      Serial.print("theAddr:");
      Serial.print(theAddr);
      Serial.print(", ptrStruct:");
      ptrValue = ptrStruct;
      Serial.println(ptrValue);

    }
    
  }
}


