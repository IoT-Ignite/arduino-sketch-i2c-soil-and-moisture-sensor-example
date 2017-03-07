#include "IgniteEsp8266ThingHandler.h"
#include "IgniteEsp8266Timer.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <I2CSoilMoistureSensor.h>
#include <Wire.h>


// Node Type
#define NODE_TYPE "I2C SOIL SENSOR"

// Sensors
#define SENSOR_I2C_SOIL "Soil Moisture Sensor"
#define SENSOR_I2C_TEMPERATURE "Temperature Sensor"
#define SENSOR_I2C_LIGHT "Light Sensor"

//Vendors
#define VENDOR_USB  "Chirp! - plant watering alarm"

// Pins
#define PIN_DATA_SDA_SCL "D1 and D2 " 

// actuator definitions
#define ACTUATOR  true
#define NOT_ACTUATOR false

//Sensor Types
#define TYPE_SOIL "Soil Moisture"
#define TYPE_TEMPERATURE "Temperature"
#define TYPE_LIGHT "Light"


//Data Type
#define DATA_TYPE_STRING "STRING"

#define CONFIG_REQUEST "configuration"
#define ACTION_REQUEST "action"
#define RESET_REQUEST "reset"
#define DATA_RESPONSE "data"
#define STATUS_REQUEST "inventory-status"

I2CSoilMoistureSensor *IgniteEsp8266ThingHandler::sensor = new I2CSoilMoistureSensor();

void IgniteEsp8266ThingHandler::setup() {

  Wire.begin();
  Serial.begin(9600);
  //writeI2CRegister8bit(0x20, 6); //reset
  sensor->begin();
  delay(100); // give some time to boot up

   
  /* Data Debug
  Serial.print("I2C Soil Moisture Sensor Address: ");
  Serial.println(sensor->getAddress(),HEX);
  Serial.print("Sensor Firmware version: ");
  Serial.println(sensor->getVersion(),HEX);
  Serial.println();
  */
  
}

IgniteEsp8266ThingHandler::IgniteEsp8266ThingHandler(): IgniteThingHandler(NODE_TYPE, getMacAddress()) {
}
IgniteEsp8266ThingHandler::~IgniteEsp8266ThingHandler() {
}

void IgniteEsp8266ThingHandler::thingActionReceived(String thingId, String action) {

/* There is no defined actuator on invertory. Don't need to handle */

}
void IgniteEsp8266ThingHandler::inventorySetup() {


  addThingToInventory(SENSOR_I2C_SOIL,
                      TYPE_SOIL,
                      PIN_DATA_SDA_SCL,
                      NOT_ACTUATOR,
                      VENDOR_USB,
                      DATA_TYPE_STRING, new IgniteEsp8266Timer(readI2CSoil));

  addThingToInventory(SENSOR_I2C_TEMPERATURE,
                      TYPE_TEMPERATURE,
                      PIN_DATA_SDA_SCL,
                      NOT_ACTUATOR,
                      VENDOR_USB,
                      DATA_TYPE_STRING, new IgniteEsp8266Timer(readI2CTemperature));

  addThingToInventory(SENSOR_I2C_LIGHT,
                      TYPE_LIGHT,
                      PIN_DATA_SDA_SCL,
                      NOT_ACTUATOR,
                      VENDOR_USB,
                      DATA_TYPE_STRING, new IgniteEsp8266Timer(readI2CLight));                                            

}


void IgniteEsp8266ThingHandler::unknownMessageReceived(String msg) {

}

String IgniteEsp8266ThingHandler::getMacAddress() {

  byte mac[6];
  WiFi.macAddress(mac);
  String cMac = "";
  
  for (int i = 0; i < 6; ++i) {
    cMac += String(mac[i], HEX);
    if (i < 5)
      cMac += ":";
  }
  cMac.toUpperCase();
  return cMac;
}

void IgniteEsp8266ThingHandler::readI2CSoil(){
  while (sensor->isBusy()) delay(50);
  String packet = "";
  String soilData = "";
  
  soilData = sensor->getCapacitance();

  StaticJsonBuffer<250> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  JsonArray& data = root.createNestedArray("data");

  root["messageType"] = DATA_RESPONSE;
  root["thingId"] = SENSOR_I2C_SOIL;
  data.add(soilData);

  root.printTo(packet);
  Serial.println("Soil :");
  Serial.println(packet);
  packet += "\n";
  sendMessage(packet);

}

void IgniteEsp8266ThingHandler::readI2CTemperature(){
  while (sensor->isBusy()) delay(50);
  String packet = "";
  String temperatureData = "";

  temperatureData = String(sensor->getTemperature()/(float) 10);

  StaticJsonBuffer<250> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  JsonArray& data = root.createNestedArray("data");


  root["messageType"] = DATA_RESPONSE;
  root["thingId"] = SENSOR_I2C_TEMPERATURE;
  data.add(temperatureData);

  root.printTo(packet);
  Serial.println("Temperature :");
  Serial.println(packet);
  packet += "\n";
  sendMessage(packet);

}

void IgniteEsp8266ThingHandler::readI2CLight(){
  while (sensor->isBusy()) delay(50);
  String packet = "";
  String lightData = "";
  
  lightData = String(sensor->getLight(true));

  StaticJsonBuffer<250> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  JsonArray& data = root.createNestedArray("data");

  root["messageType"] = DATA_RESPONSE;
  root["thingId"] = SENSOR_I2C_LIGHT;
  data.add(lightData);

  root.printTo(packet);
  Serial.println("Light :");
  Serial.println(packet);
  packet += "\n";
  sendMessage(packet);

}
