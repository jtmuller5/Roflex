#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <wifi/AdafruitIO_WINC1500.h>
#ifdef __AVR__
#include <avr/power.h>
#include <RoflexPins.h>
#include <Adafruit_MQTT.h>
#include "Adafruit_MQTT_Client.h"
#endif
