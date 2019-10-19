#include <RoflexLibs.h>
#include <RoflexAnimations.h>

/*
Title:
Definition:
Parameters:
*/

/*
Title: WhatsUp
Definition: This function takes the z value from the orientation sensor and determines which of the 8 LEDs
            is facing upward. It will then light up that LED and turn all others off
Parameters: z - Z value from orientation sensor that determines rotation about a central axis
*/

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

/*
Title: freestyle
Definition: Changes the color of the LEDs based on a character sent by the user over bluetooth
Parameters: Command - A single character that determines what color to change the LEDs to
*/

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

/*
Title: timer
Definition: (Not Finished) Light up the LEDs when a certain interval has passed, turn off the LEDs after another interval has passed
            Start with the LEDs off
Parameters: on - The number of seconds that the LEDs should be turned on for
            off - The number of seconds the LEDs should be turned off for
*/

void timer(int on,int off){

}

/*
Title: AngleFinder
Definition: Function that changes the color of the LEDs based on the angle of elevation.
            At 0 degrees, the LEDs should be green. As you move away from being perfectly horizontal, the LEDs will gradually change to red
Parameters: y - Angle from horizontal. +90 is straight up, -90 is straight down
*/

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

/*
Title: displayCalStatus
Definition: Displays the calibration status of the device
Parameters: none
*/

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

/*
Title: saveFront
Definition: This function is used to save a front-facing direction. It works by continuously saving 400 direction values into an array
            and then calculating the standard deviation of those values. If the standard deviation is small, we assume the user
            is steady and this angle is saved to the global "front" variable.

            This angle is then used to determine if the user's form is correct
Parameters: none (might want to add variables for the number of values collected and the standard deviation that determines steadiness)
*/

void saveFront(void){
  if (frontSaved == false) {
    float total = 0;
    int headerVals [400]; //Create integer array to hold temporary sensor values for direction, a larger array should make the user remain stable for longer

    //Each time the device resets and needs to calibrate, turn the neopixels 'off'
    for (uint16_t i = 0; i < 3; i++) {
      strip.setPixelColor(i, 0, 0, 0);
      strip.show();
    }

    //The device will fill the headerVals and angleVals arrays with sensor derived numbers
    for (int i = 0; i <= 399; i++) {
      sensors_event_t event;
      bno.getEvent(&event);
      float x = event.orientation.x; //Gives angle from north

      headerVals[i] = x;
      total += headerVals[i];  //Each value that is added to the arrays is added to the total variables
    }

    //The total is then divided by the total number of values in the array to find the average
    //This value can then be used as a reference for the remainder of the exercise
    front = total / 400;

    //To find the standard deviation of the array values, the diffrence between each value and the average will be calculated and summed
    float sqDevSum = 0.0;

    for (int i = 0; i <= 399; i++) {
      sqDevSum += pow((front - float(headerVals[i])), 2);
    }

    //The sum will then be divided by the total number of values in the array
    float stDev = sqrt(sqDevSum / 100);

    //If the standard deviation is below a given threshold (.5 in this case), the calibration value will become true
    if ((stDev < .5) & (frontSaved == false)) {
      frontSaved = true;
      spinLights(127, 0, 100);
      mode = lastmode;
    }
    //If not, calibration remains false and the device will repeat this process
    else if (stDev > .5) {
      frontSaved = false;
    }
  }
}

/*
Title: checkFront
Definition: If a front value has been saved, this function will direct the user to that front direction
            using the LEDs. Special handling is added to account for the 360 - 0 transition that happens to compass values
Parameters: x - X orientation value from sensor (compass direction)
*/

void checkFront(float x){
  boolean rotateCW; //Variable to set rotation direction
  if (front == -1){
    saveFront();
  }
  else {
    // Adjust for 360 to 0 jump for compass values
    if(front+180>360){
      float overlap = 180 - (360 - front);
      if((x>=front)|(x<overlap)){ //If we are to the right of the front direction...
        rotateCW = false;
      }
      else {
        rotateCW = true;
      }
    }

    else {
      float overlap = 360 - (180-front);
      if((x<=front)|(x>overlap)){ //If we are to the left of the front direction...
        rotateCW = true;
      }
      else {
        rotateCW = false;
      }
    }
    rotate(rotateCW,127,127,100);
  }
}
