#include "RoflexLibs.h"
#include "RoflexPins.h"
#include "RoflexMotion.h"
#include <Adafruit_MQTT.h>

void setup(void)
{
  //Start communication on both Serial (USB) and Serial1 (bluetooth)
  Serial.begin(9600);
  BSERIAL.begin(9600);
  WSERIAL.begin(115200);
  Serial.println("Orientation Sensor Test"); Serial.println("");

  //Begin neopixel data flow and initialize all pixels to 'off'
  strip.begin();
  strip.setBrightness(65);
  strip.show();

  /* Initialise the sensor */
  if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  delay(1000);
  bno.setExtCrystalUse(true);
}


void loop(void)
{
  while(BSERIAL.available()){
   delay(10);
   char c = BSERIAL.read();
   command += c;
 }

 if(command != ""){
   menu1 = command.charAt(0); //First tier option
   menu2 = command.charAt(1); //Second tier option
   Serial.println(menu1);
   Serial.println(menu2);
 }
 //test

  // if(command.length() > 0){
  //   if(command == "5" & collectPos == true) collectPos = false;
  //   else if(command == "5" & collectPos == false) collectPos = true;
  //   else if(command == "1" & mode < 4){
  //     mode = mode + 1;
  //   }
  //   else if(command == "1" & mode == 4){
  //     mode = 1;
  //   }
  // }

// Collect orientation data for use in any of the following functions
if(collectPos == true){
  sensors_event_t event;
  bno.getEvent(&event);
  imu::Vector<3> gravity = bno.getVector(Adafruit_BNO055::VECTOR_GRAVITY);

  x = event.orientation.x; //Gives angle from north (I think this is actually a relative direction value)
  y = event.orientation.y; //Gives angle from horizontal, +90 up, -90 down
  z = event.orientation.z; //Gives angle of rotation along device, +180 CCW, -180 CW

  vx = gravity.x(); //Gravity along the x-axs
  vy = gravity.y(); //Gravity along the y-axis
  vz = gravity.z(); //Gravity along the z-axis

  xa = (vx/9.81)*90; //Angle of elevation along the x-axis, derived from gravity vector

/* Display the floating point data */
Serial.print("Front: ");
Serial.print(mode);
Serial.println("");
}
//Set mode based on BT input
if(menu1 == 'q'){ //q = quick functions
  if(menu2 == 'u'){
    mode = 1;
    lastmode = 1;
  }
  else if(menu2 == 'a'){
    mode = 2;
    lastmode = 2;
  }
  else if(menu2 == 'c'){
    mode = 3;
    lastmode = 3;
  }
  else if(menu2 == 'f'){
    mode = 4;
    lastmode = 4;
  }
  else if(menu2 == 't'){
    mode = 5;
    lastmode = 5;
  }
  else if(menu2 == 'o'){
    mode = 6;
    lastmode = 6;
  }
}

//All settings functions should exit by setting the mode to the lastMode variables
//Otherwise, you get stuck in the settings function
else if(menu1 == 's'){ //s = settings
  if(menu2 == 'c'){
    frontSaved = false;
    mode = 7;
  }
}

    switch(mode){
      case 1:
        WhatsUp(z);
        break;
      case 2:
        AngleFinder(y);
        break;
      case 3:
        displayCalStatus();
        break;
      case 4:
        freestyle(command);
        break;
      case 5:
        timer();
        break;
      case 6:
        checkFront(x);
        break;
      case 7:
        saveFront();
        break;
      default:
        break;
      }

 //Reset inputs so we don't get stuck
  menu1 = "";
  menu2 = "";
  command = "";
}
