## Files for the sparki robot

For now most of the stuff is play :)

## NOTE
- I tried to make common code (include files) but couldn't get arduino ide to recognize the path to different directories.  What I did is have the code for an area in a .h file in the directory for that area (i.e localizationCommon.h is in the localizationUtils directory).  To use that code in a different area (i.e. determineWorld) you copy the .h file into the determineWorld directory.  I created a 'syncFiles.sh' script in git/root path, when you run it it copies the .h files to the directories that need them.  When you write a new area just update it to include the .h files you want to use.  MAKE SURE you keep it up to date, and only update the .h file in the correct location.  Also, you need to close/open the folder after you've updated a file and have the the shell script (the ide doesn't know if underlying file was updated). **TIME PERMITTING** change each of the repective areas to be a class.  Here's a brief overview
  - **sparkiCommon.h** has common definitions and global variable definitions
  - **localizationCommon.h** code related to localization (more below)

## To Do's
- If have time change code to use classes to better encapsulate things
- Create a 'wander world' or explore world routine
- Create routine to follow wall
- Test light logic, to find light may want to use shadow logic; see if light is affected by other sensors, magnetometer...
- Have a colision method
- Have a battery life state
- Remember can use struct and define var as int : 3; to mean use three bits 0-7 values

## Descriptions of Code
- **movementTest**: This has code used to test movements; I used this to calculate how fast sparki is (2.78cm/sec); to do this you can use the moveForward function, it'll show the elapsed time to move that distance.  I found that sparki was off; it actually moved further than requested.  See the sparkiMovementAnalysis spreadsheet in the sparki_python repo for calculations I did.

- **localizationUtils**: this has code for localization, the main program is just a test.  The meat is in localizationCommon.h, the methods of interest
  - **setCurrentLocalizationValues(x,y,angle)** this sets your current location and angle of orientation (orientation in the world)
  - **setNewPosition(distance, angleFromLastPosition)** you give it the distance you traveled and the angle from your last position and it calculates your new world model position.  Note that angle from last position is relative; lets say your prior position was at 10,3 and you were at a 10' angle.  If you traveled 5cm at a 20' angle, the 20' is relative angle to your last position, but your angle in the 'world' is 20' + last orientation angle (10), so it's 30'.  fyi, your new position is at 14.3,5.5
  - **getShortestAngleDeltaToGetToOrientation** you give this routine the real world angle orientation and it returns the shortest angle delta needed to point there... if the angle returned is negative then turn right, if positive turn left. 
  - **setCurrentAngle(angleInWorld)** use this to set your angle in the world
  - **getCurrentAngle()** returns world angle
  - **setCurrentXPosition(x)** sets your current x coordinate position 
  - **getCurrentXPosition()** returns your x coordinate position
  - **setCurrentYPosition(x)** sets your y position
  - **getCurrentYPosition()** returns your y position
  - **calculateNewX(distance, angleFromLastPosition)** returns what x position is after traveling... this does not update your current position; only mention here as may want
  - **calculateNewY(distance, angleFromLastPosition)** similar to routine above, but for y position
  - 
  
- **determineWorld**: this has code to calculate the size or your world, it will update the 'worldXDimension' and 'worldYDimension' and also call the method in the localization to set the current x, y and angles.  (remember to get these you can call getCurrentXPosition(), getCurrentYPosition() and getCurrentAngle()).  Some routines of interest:
  - **getWorldXDimension()** returns the size of the world - x's dimension
  - **getWorldYDimension()** returns the y dimension of our world
  - **adjustWorldCoordinate(x, y)** use this to adjust your world... the original calculation only suppors the range of the ultrsonic sensor, if your world is larger than that you can call this routine to adjust it
  - **calculateRectangularCoordinates()** routine calculates rectangular coordinates of the world... just reads ultrasonic sensor at each 90' and derives what the overall world is; it then calculates it's position within it.  You can use getWorldXDimension (or y) to get world dimensions, you can use the getCurrentXPosition() (or y) to get robots current position and getCurrentAngle() to get it's angle of orientation.


## LEGEND Serial communications
- **DW** Define world, world dimensions (x,y)
- **DP** Define world, current position (x,y)
- **LO** Localization, x position, y position, &lt; current angle