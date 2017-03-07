#ifndef _INCL_IGNITE_ESP8266THING_HANDLER
#define _INCL_IGNITE_ESP8266THING_HANDLER

#include "IgniteThingHandler.h"
#include "IgniteEsp8266ThingHandler.h"
#include <I2CSoilMoistureSensor.h>
#include <Wire.h>

class IgniteEsp8266ThingHandler : public IgniteThingHandler {

  public :

    IgniteEsp8266ThingHandler();
    ~IgniteEsp8266ThingHandler();
    virtual void thingActionReceived(String thingId, String action);
    virtual void inventorySetup();
    virtual void unknownMessageReceived(String msg);
    static void readI2CSoil();
    static void readI2CTemperature();
    static void readI2CLight();
    virtual void setup();

  private :
    static I2CSoilMoistureSensor *sensor;
    String getMacAddress();
  };

#endif
