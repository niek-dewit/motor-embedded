#include <Arduino.h>
#include <SSD1803A_I2C.h>
#include "./services/chargingService.h"
#include "./services/interfaceService.h"
#include <memory>

std::unique_ptr<ChargingService> chargingService = std::make_unique<ChargingService>();
std::unique_ptr<InterfaceService> interfaceService = std::make_unique<InterfaceService>(chargingService.get());

void setup() {
  Serial.begin(9600);
}

void loop() {
  chargingService->loop();
  interfaceService->loop();
}
