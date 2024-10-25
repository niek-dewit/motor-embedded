#include "interfaceService.h"
#include <Arduino.h> 

const uint8_t sdaPin = 18;
const uint8_t sclPin = 19;
const long displayInterval = 100;

InterfaceService::InterfaceService(ObcService *ObcService) : displays(), pages() {
    registerPage(EmptyPage::ID, std::make_unique<EmptyPage>());
    registerPage(ChargingInfoPage::ID, std::make_unique<ChargingInfoPage>(ObcService));

    registerDisplay(Display::DISPLAY2, Display::resetPinDisplay2);
    registerDisplay(Display::DISPLAY1, Display::resetPinDisplay1);

    pinMode(sdaPin, INPUT_PULLUP);
    pinMode(sclPin, INPUT_PULLUP);

    displays[Display::DISPLAY1]->setPage(pages[ChargingInfoPage::ID].get());

}

void InterfaceService::registerDisplay(u_int8_t displayId, u_int8_t resetPin) {
  displays[displayId] = std::make_unique<Display>(displayId, resetPin, pages[EmptyPage::ID].get());
}

void InterfaceService::registerPage(u_int8_t pageId, std::unique_ptr<Page> page) {
  pages[pageId] = std::move(page);
}
bool switchScreens = false;
void InterfaceService::loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= displayInterval) {
    previousMillis = currentMillis;
    refresh();
  }

  bool checkSwitchScreens = currentMillis /3000 % 2;
  if(switchScreens != checkSwitchScreens) {
    switchScreens = checkSwitchScreens;  
    if(switchScreens) {
      displays[Display::DISPLAY1]->setPage(pages[ChargingInfoPage::ID].get());
      displays[Display::DISPLAY2]->setPage(pages[EmptyPage::ID].get());
    } else {
      displays[Display::DISPLAY2]->setPage(pages[ChargingInfoPage::ID].get());
      displays[Display::DISPLAY1]->setPage(pages[EmptyPage::ID].get());
    }
  };
}

void InterfaceService::refresh() {
  for (const auto& [key, display] : displays) {
    display->refresh();
  }
}
