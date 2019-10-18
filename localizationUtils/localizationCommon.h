// #include <Sparki.h>
//const float PI = 3.141592654;

float lastXPosition;
float lastYPosition;
float lastAngleOfOrientation;

void setCurrentLocalizationValues(float x, float y, float angle) {
  lastXPosition = x;
  lastYPosition = y;
  lastAngleOfOrientation = angle;
}

float degreesToRadians(float degrees) {
  return (degrees * (PI / 180.0));
}

// This is basically mod(angle,360.0), couldn't do it with % and float args so wrote my own
float getAngle(float angleInDegrees) {
  if (angleInDegrees >= 360.0)
    return getAngle(angleInDegrees - 360.0);
  else
    return angleInDegrees;
}

// Common function to return our 'real angle'; it's the angleDelta + prior orientation (it's degrees)
float calculateRealAngleWithAdjustment(float angleDelta) {
  return getAngle(angleDelta + lastAngleOfOrientation);
}

float calculateNewX(float distance, float angleDelta) {
  // Angle delta is the angle relative to the last position, the real angle is that
  // value plus the lastAngleOfOrientation (the angle it was at when we started).
  // remember cos is adjacent/hypotenuse
  // xValue is calculated by 
  // .   calculate the x alone (it's cos(angle)*distanceTraveled
  //     add the x alone to the lastRobot x position
  return ((cos(degreesToRadians(calculateRealAngleWithAdjustment(angleDelta))) * distance) + lastXPosition);
}

float calculateNewY(float distance, float angleDelta) {
  // Similar to the logic for x, but we use sin here (remember sin is Opposite/Hypotenuse)
  // Angle delta is the angle relative to the last position, the real angle is that
  // value plus the currentAngle (the angle it was at when we started).
  return ((sin(degreesToRadians(calculateRealAngleWithAdjustment(angleDelta))) * distance) + lastYPosition);
}

void setNewPosition(float distance, float angleDelta) {
  float newX = calculateNewX(distance, angleDelta);
  float newY = calculateNewY(distance, angleDelta);
  // Set new location
  setCurrentLocalizationValues(newX, newY, calculateRealAngleWithAdjustment(angleDelta));
}
 
// For debugging we may want to show values on lcd screen
void showLoc() {
  sparki.clearLCD(); // wipe the LCD clear
  sparki.print("x: ");
  sparki.print(lastXPosition);
  sparki.print(" y: ");
  sparki.print(lastYPosition);
  sparki.print(" angle: ");
  sparki.println(lastAngleOfOrientation);
  sparki.updateLCD(); // put the drawings on the screen
}




