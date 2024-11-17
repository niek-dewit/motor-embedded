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
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  interfaceService.logging = false;
}


int heartBeatInterval = 1000;
int prevHeartBeat = 0;
bool blinking = false;
void loop() {
  interfaceService.loop();
  canBusService.loop();

  if (millis() - prevHeartBeat > heartBeatInterval) {
    //Serial.print("Heartbeat: "); Serial.println(millis());
    prevHeartBeat = millis();
    blinking = !blinking;
    if (blinking) {
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }
    
  }



}
