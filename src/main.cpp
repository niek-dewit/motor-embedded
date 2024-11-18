#include <Arduino.h>
#include <SSD1803A_I2C.h>
#include "./services/obc/obcService.h"
#include "./services/bms/bmsService.h"
#include "./services/dcdc/dcdcService.h"
#include "./services/controller/controllerService.h"
#include "./services/displays/displaysService.h"
#include "./services/canBus/canBusService.h"
#include "./services/buttons/buttonsService.h"
#include "./services/heartbeat/heartbeatService.h"
#include "./pages/obcState/obcStateInfo.h"
#include "./pages/chargingPort/chargingPortInfo.h"
#include "./pages/bmsVoltagesInfo/bmsVoltagesInfo.h"
#include "pages/empty/empty.h"
#include <memory>


ObcService& obcService = ObcService::getInstance();
DcdcService& dcdcService = DcdcService::getInstance();
BmsService& bmsService = BmsService::getInstance();
ControllerService& controllerService = ControllerService::getInstance();
DisplaysService& displaysService = DisplaysService::getInstance();
ButtonsService& buttonsService = ButtonsService::getInstance();
CanBusService& canBusService = CanBusService::getInstance();
HeartbeatService& heartbeatService = HeartbeatService::getInstance();

bool state = false;
long lastSwitch = 0;

void displayState1() {
  displaysService.registerPage(
    DisplaysService::DisplayAddress::DISPLAY2,
    std::make_unique<BmsVoltagesInfoPage>(1)
  );

  displaysService.registerPage(
    DisplaysService::DisplayAddress::DISPLAY1,
    std::make_unique<ChargingPortInfoPage>()
  );
}

void displayState2() {
  displaysService.registerPage(
    DisplaysService::DisplayAddress::DISPLAY2,
    std::make_unique<EmptyPage>()
  );

  displaysService.registerPage(
    DisplaysService::DisplayAddress::DISPLAY1,
    std::make_unique<ObcStateInfoPage>()
  );
}



void setup() {
  Serial.begin(115200);
  displaysService.logging = false;

  displaysService.setReadyCallback(std::make_unique<std::function<void()>>( []() {
    
    displayState2();
    buttonsService.getButton(ButtonsService::ButtonPin::BUTTON1).setCallback(std::make_unique<std::function<void(bool)>>( [](bool pressed) {
      if(lastSwitch + 3000 < millis()) {
        if(!state) {
          displayState1();
        } else {
          displayState2();
        }
        state = !state;
        lastSwitch = millis();
      }


    }));
  }));

}

void loop() {
  displaysService.loop();
  canBusService.loop();
  buttonsService.loop();
  heartbeatService.loop();
}
