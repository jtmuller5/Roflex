#include <RoflexLibs.h>
void WhatsUp(float z){
  /* Get a new sensor event */
  strip.clear();

  if (z>=-22.5 && z <=22.5){
    strip.setPixelColor(0,255,255,0);
  }
  if (z>=-67.5 && z <-22.5){
    strip.setPixelColor(7,255,255,0);
  }
  if (z>=-112.5 && z <-67.5){
    strip.setPixelColor(6,255,255,0);
  }
  if (z>=-157.5 && z <-112.5){
    strip.setPixelColor(5,255,255,0);
  }
  if (z>157.5 || z <-157.5){
    strip.setPixelColor(4,255,255,0);
  }
  if (z>22.5 && z <=67.5){
    strip.setPixelColor(1,255,255,0);
  }
  if (z>67.5 && z <=112.5){
    strip.setPixelColor(2,255,255,0);
  }
  if (z>112.5 && z <=157.5){
    strip.setPixelColor(3,255,255,0);
  }

  strip.show();
}

void freestyle(String command){
  Serial.println(command);
  if(command == "r"){
    strip.fill(red);
  }
  else if(command == "y"){
    strip.fill(yellow);
  }
  else if(command == "b"){
    strip.fill(blue);
  }
  else if(command == "g"){
    strip.fill(green);
  }
  strip.show();
}

void timer(){
  Serial.println(command);
  if(command == "r"){
    strip.fill(red);
  }
  else if(command == "y"){
    strip.fill(yellow);
  }
  else if(command == "b"){
    strip.fill(blue);
  }
  else if(command == "g"){
    strip.fill(green);
  }
  strip.show();
}

void AngleFinder(float y){
  float redVal;
  float greenVal;
  if(abs(y) > 45){
    redVal = 255;
    greenVal = 255-(((abs(y)-45)/45)*255);
  }
  else if(abs(y) <= 45){
    redVal = (abs(y)/45)*255;
    greenVal = 255;
  }

  Serial.println(y);
  Serial.println(redVal);
  Serial.println(greenVal);
  uint32_t grade = strip.Color((int)redVal,(int)greenVal,0);
  strip.fill(grade);
  strip.show();
}

void displayCalStatus(void)
{
  /* Get the four calibration values (0..3) */
  /* Any sensor data reporting 0 should be ignored, */
  /* 3 means 'fully calibrated" */
  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  /* The data should be ignored until the system calibration is > 0 */
  Serial.print("\t");
  if (!system)
  {
    Serial.print("! ");
  }

  /* Display the individual values */
  Serial.print("Sys:");
  Serial.print(system, DEC);
  Serial.print(" G:");
  Serial.print(gyro, DEC);
  Serial.print(" A:");
  Serial.print(accel, DEC);
  Serial.print(" M:");
  Serial.println(mag, DEC);
}
