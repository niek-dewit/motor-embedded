#include <Arduino.h>
#include <SSD1803A_I2C.h>
#include "./services/obc/obcService.h"
#include "./services/bms/bmsService.h"
#include "./services/dcdc/dcdcService.h"
#include "./services/controller/controllerService.h"
#include "./services/interface/interfaceService.h"
#include <memory>
ObcService& obcService = ObcService::getInstance();
DcdcService& dcdcService = DcdcService::getInstance();
BmsService& bmsService = BmsService::getInstance();
ControllerService& controllerService = ControllerService::getInstance();

InterfaceService& interfaceService = InterfaceService::getInstance();
CanBusService& canBusService = CanBusService::getInstance();

void setup() {
  Serial.begin(115200);
}

int heartBeatInterval = 1000;
int prevHeartBeat = 0;

void loop() {
  interfaceService.loop();
  canBusService.loop();

  if (millis() - prevHeartBeat > heartBeatInterval) {
    Serial.print("Heartbeat: "); Serial.println(millis());
    prevHeartBeat = millis();
  }

}
