#include <RoflexLibs.h>
#include <Adafruit_MQTT.h>

#define USE_WINC1500

//#include <AdafruitIO_WiFi.h>

//File for storing pin assignments and global variables

// NeoPixel pin and number of LEDs
#define LED_PIN 12
#define LED_COUNT 8
#define BSERIAL Serial1 //Establish that communication through pins 0 and 1
#define WSERIAL Serial2 //Wifi communication through ESP8266 chip

int button = 22; //Pin for reset
int cycle = 21; //Pin for cycling functions
char received = '0'; //Variable for toggling by phone

//WiFi setup
#define IO_USERNAME   "Jtmuller"
#define IO_KEY        "c36a6d29ce804f3f9fb1563092a07767"
#define WIFI_SSID   "Not Secure"
#define WIFI_PASS   "MoneyMan5"
#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883

//AdafruitIO_WiFi io = io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

//Create MPU object
Adafruit_BNO055 bno = Adafruit_BNO055(55);

//---------------------------------------------------------
//MENUS
//---------------------------------------------------------
//Primary Menu
String primaryMenu[] = {"Settings","Activities"};
int primMenuLen = 2;
int primFunction = 1; //Variable to hold the current primary function

//Secondary Menus
String settingsMenu[] = {"Lights","Other"};
int setMenuLen = 2;
int settingFunction = 1; //Variable to hold the current setting functions
String activitiesMenu[] = {"Timed","Form"};
int actMenuLen = 2;
int quickFunction = 1; //Variable to hold the current primary quick function

//Tertiary Menus
String lightsMenu[] = {"Colors","Animation","Brightness"};
int ligMenuLen = 3;
int lightFunction = 1; //Variable to hold the current light settingFunction
String otherMenu[] = {"Other"};
int othMenuLen = 1;
int otherFunction = 1; //Variable to hold the current settingFunction
String timedMenu[] = {"Countdown","Interval"};
int timMenuLen = 2;
int timedFunction = 1; //Variable to hold the current timed activity
String formMenu[] = {"WhatsUp","WhatsNorth"};
int formMenuLen = 2;
int formFunction = 1; //Variable to hold the current form activity

//---------------------------------------------------------
//GLOBAL VARIABLES
//---------------------------------------------------------
//Mode selection variables
String command; //Command that is assembled from individual bytes
int mode = 1;
int lastmode = 1; //Value to hold the previous mode. Will be used to exit setting functions
char menu1;
char menu2;

boolean collectPos = true; //Toggle to turn orientation collection on and off

//Reference variables for correct movement
float front = -1; //Holds the front facing direction of the user (calculated in saveFront function)
unsigned long startTime;
boolean frontSaved = false;

// Orientation values
float x;
float y;
float z;
// Gravity vector values
float vx;
float vy;
float vz;
float xa; //Pitch angle determined from gravity vector

//---------------------------------------------------------
//NEOPIXEL VALUES
//---------------------------------------------------------
uint32_t red = strip.Color(240, 47, 22);
uint32_t blue = strip.Color(40, 19, 235);
uint32_t yellow = strip.Color(235, 227, 19);
uint32_t green = strip.Color(13, 217, 40);

//String *primMenuPointer[] = &primaryMenu;
//String *secMenuPointer[] = &settingsMenu;
//String *terMenuPointer[] = &lightsMenu;
