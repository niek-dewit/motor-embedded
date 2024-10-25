#include <Arduino.h>
#include <SSD1803A_I2C.h>
#include "./services/obc/obcService.h"
#include "./services/bms/bmsService.h"
#include "./services/dcdc/dcdcService.h"
#include "./services/controller/controllerService.h"
#include "./services/interface/interfaceService.h"
#include <memory>
std::unique_ptr<ObcService> obcService = std::make_unique<ObcService>();
std::unique_ptr<BmsService> bmsService = std::make_unique<BmsService>();
std::unique_ptr<DcdcService> dcdcService = std::make_unique<DcdcService>();
std::unique_ptr<ControllerService> controllerService = std::make_unique<ControllerService>();

std::unique_ptr<InterfaceService> interfaceService = std::make_unique<InterfaceService>(obcService.get());
CanBusService& canBusService = CanBusService::getInstance();

void setup() {
  Serial.begin(115200);
}

void loop() {
  interfaceService->loop();
  canBusService.loop();
}
