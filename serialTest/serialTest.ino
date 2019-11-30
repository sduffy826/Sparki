#include <Sparki.h>  // include the sparki library

#define DEBUGINFRARED true

String inputString; 
int counter;

struct Pose {
  float xPos;
  float yPos;
  int angle;
};

struct InfraredInstructions {
  // InfraredInstructions();
  char instruction;  // X-explore, G-goal, Q-quit/done M-moveto (goto)
  Pose pose;         // has x, y (floats) and angle (int)
};

// -------------------------------------------------------------------------------------
void setup()
{
   Serial.begin(9600);
   sparki.beep();
   delay(5000);  
   sparki.beep();
   Serial.setTimeout(10000);  // Set serial timeout to 10 seconds
   inputString = "";
   counter = 0;

   // Clear anything on the serial port
   if (Serial.available() > 0)  {
     sparki.RGB(RGB_GREEN);
     while (Serial.available() > 0) {
       char theChar = (char)Serial.read(); 
       delay(5);
     }
   }
   else sparki.RGB(RGB_BLUE);
   sparki.beep();
}

// -------------------------------------------------------------------------------------
// Read a character from the serial port, we'll wait 10 seconds
int readFromSerialPort() {
  int theCnt = 0;
  while (theCnt < 10000) { 
    if (Serial.available() > 0) {
      return (int)Serial.read();
    }
    theCnt +=5;
    delay(5);
  }
  return -1;
}

// -------------------------------------------------------------------------------------
const char* extractToken(char* dst, const char* src) {
  while ( *src ) {
    if ( ',' == *src ) {
      *dst = '\0';
      return ++src;
    }
    *dst++ = *src++;
  }
  *dst = '\0';
  return NULL;
}

// -------------------------------------------------------------------------------------
bool waitForInstructions() {
  
  Serial.println("IR,INS");

  // Python will first tell us how many characters will follow we'll use that to allocate the array
  // The routine below will wait up to 10 seconds
  int bytes2Read = readFromSerialPort();
  Serial.print("read:");
  Serial.print(bytes2Read); Serial.print(" ");
  char *str_data = NULL;
  if (bytes2Read > 0) {
    char inputBuffer[bytes2Read+1];
    int indexPos       = 0;
    int maxLen         = 0;
    int num_parameters = 0;
    int currLen        = 0;
    int valueRead;
    while (indexPos < bytes2Read) {
      valueRead = readFromSerialPort();
      Serial.print(valueRead); Serial.print(" ");
      if (valueRead < 0) {
        // Didn't get a value... we'll exit
        bytes2Read = 0; 
      }
      else {        
        inputBuffer[indexPos] = (char)valueRead;
        if (inputBuffer[indexPos] == ',') {
          if (currLen > maxLen) 
            maxLen = currLen;
          currLen = 0;
          num_parameters++;
        }
        else currLen++;
        indexPos++;
      }
    }
    inputBuffer[indexPos] = '\0';
    Serial.print("c1");
    Serial.println(bytes2Read);
 
    // Take any junk off port
    while (Serial.available() > 0) {
      Serial.read();
      delay(2);
    }
//bytes2Read = 0;
    // bytes2Read is set to zero if encountered a problem
    if (bytes2Read > 0) {
      // Process the last record from above... if currLen is > 0 we need to process it
      if (currLen > 0) {
        num_parameters++;
        if (currLen > maxLen) 
          maxLen = currLen;
      }
      // Assign pointer to address of array (same as &str_data = &inputBuffer[0];)
      str_data = inputBuffer;
 
      // Now put into string array
      //const size_t PARAMS_MAX = num_parameters;
      //const size_t LENGTH_MAX = len_parameter;
      //char szParams[PARAMS_MAX][LENGTH_MAX + 1];
      char szParams[num_parameters][maxLen+1];
  
      // This setups the 0'the row and sets pnext to point to the next address
      char* pnext = (char*)extractToken(&szParams[0][0], str_data);
      for (indexPos = 1; indexPos < num_parameters; indexPos++) {  
         pnext = (char*)extractToken(&szParams[indexPos][0], pnext);
      }
    
      if (DEBUGINFRARED) {
        for ( size_t i = 0; i < num_parameters; i++ ) {
           Serial.print("szParams[");
           Serial.print(i);
           Serial.print("]:");
           Serial.println(szParams[i]);
        }
      }
    
      // Put the instructions into the stack
      indexPos = 0;
      InfraredInstructions theInstructions;
      while (indexPos < num_parameters) {
        switch( indexPos % 7) {
          case 0:
            theInstructions.instruction = szParams[indexPos][0];
            //Serial.print("s$");
            //Serial.println(szParams[indexPos][0]);
            if (DEBUGINFRARED) {
              Serial.print("ins:");
              Serial.println(theInstructions.instruction);
            }
            break;
          case 2: // x
            theInstructions.pose.xPos = atof(szParams[indexPos]);
            if (DEBUGINFRARED) {
              Serial.print("xPos:");
              Serial.println(theInstructions.pose.xPos);
            }
            break;
          case 4: // y
            theInstructions.pose.yPos = atof(szParams[indexPos]);
            if (DEBUGINFRARED) {        
              Serial.print("yPos:");
              Serial.println(theInstructions.pose.yPos);
            }
            break;
          case 6: // angle
            theInstructions.pose.angle = atoi(szParams[indexPos]);
            if (DEBUGINFRARED) {        
              Serial.print("<:");
              Serial.println(theInstructions.pose.angle);
            }
            //queueOfInstructions.push(theInstructions);
            break;
        }
        indexPos++;
      }
      return true;
    }
    else
      return false;
  }
  else
    return false; // Serial.println("No records to read on serial port");
}

// -------------------------------------------------------------------------------------
// This doesn't work because the array is released when the function completes... note
// you may still see that in memory, but it will get clobbered eventually 
char * rtnArray() {
  char seany[6];

  unsigned long theAddr = &seany[0];
  seany[0] = 'S';
  seany[1] = 'e';
  seany[2] = 'a';
  seany[3] = 'n';
  seany[4] = 'y';
  seany[5] = '\0';
  char *ptr;
  ptr = seany;

  for (int i = 0; i < 5; i++) {
    Serial.print(*ptr++);
  }
  Serial.println(" ");
  Serial.println(theAddr);
  ptr = seany;
  theAddr = ptr;
  Serial.print(theAddr);
  //longAddr = *ptr;
  //Serial.print(*ptr);
  return ptr;
}

 // -------------------------------------------------------------------------------------
void loop()
{  
  
  if (counter == 0) {
    counter = counter + 1;
    
    if (true == false) {
      sparki.beep();
      Serial.println("Sending data to serial, serial send back");
      Serial.print(counter);
      Serial.print(" ");
      Serial.print(Serial.available());
      Serial.println(" R - Restart, X - eXit");
      delay(100);
      sparki.beep();
    }
    
  /*
  if (Serial.available() > 0)  {
    int inByte = Serial.read();
    char theChar = (char)inByte;
    if (theChar != 'R' && theChar != 'X') {
      inputString += theChar;
    }
    else {
      Serial.print("Read: ");
      Serial.println(inputString);
      inputString = "";
      counter = 0;
    }
  }
  */
    if (true == false) {
      char *ptr2Char = rtnArray();
      unsigned long theAddr = ptr2Char;
      for (int i = 0; i < 5; i++) {
        Serial.print(*ptr2Char++);
      }
      Serial.println(" ");
      Serial.print("mainline, theAddr: ");
      Serial.println(theAddr);
    }

    if (true == true) {
      if (waitForInstructions()) Serial.print("waitForInstructions returned true");
      else Serial.println("waitForInstructions returned false");
    }
  
    if (true == false) {
      Serial.println("Start of test");
      
      int bytes2Read = readFromSerialPort();
      if (bytes2Read > 0) {
        char inputBuffer[bytes2Read+1];
        int indexPos       = 0;
        int maxLen         = 0;
        int num_parameters = 0;
        int currLen        = 0;
        while (indexPos < bytes2Read) {
          inputBuffer[indexPos] = (char)readFromSerialPort();
          if (inputBuffer[indexPos] == ',') {
            if (currLen > maxLen) 
              maxLen = currLen;
            currLen = 0;
            num_parameters++;
          }
          else currLen++;
          indexPos++;
        }
        if (currLen > 0) {
          num_parameters++;
          if (currLen > maxLen) 
            maxLen = currLen;
        }
        inputBuffer[indexPos] = '\0';
         
        // Take any junk off port
        while (Serial.available() > 0) {
          Serial.read();
          delay(2);
        }
        // Send it back
        // for (int i = 0; i < bytes2Read; i++) Serial.print(inputBuffer[i]);
        // Serial.println(" ");

        const char *str_data = inputBuffer;
        for (int i = 0; i < bytes2Read; i++) Serial.print((char)*str_data++);
        Serial.println(" ");
        Serial.print("maxLen: "); Serial.println(maxLen); 
        Serial.print("num_parameters: "); Serial.println(num_parameters);
      }
      else Serial.println("No records to read on serial port");

      /*
      Serial.print("out of loop");
      char theChar = *str_data;
      while (theChar != '\0') {
        Serial.println(theChar);        
        theChar = ++*str_data;
      }
      */
      
     // while (theChar = *str_data++) {
     //   Serial.print(theChar);
     // }
      Serial.println(" ");
    }
  }
  
}
