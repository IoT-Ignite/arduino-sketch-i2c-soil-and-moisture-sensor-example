#include <Arduino.h>
#include "IgniteEsp8266WifiManager.h"
#include "IgniteEsp8266ThingHandler.h"

IgniteEsp8266ThingHandler handler;
IgniteEsp8266WifiManager manager(&handler);

void setup() {
  Serial.println(WiFi.localIP());
  manager.setup();
}
void loop() {
  manager.loop();
}
