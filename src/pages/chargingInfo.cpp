
#include "page.h"
#include <Arduino.h> 
#include "../devices/display.h"
#include <memory>

ChargingInfoPage::ChargingInfoPage(Display *display) :
  Page(ChargingInfoPage::ID, display) {
  Serial.println("ChargingInfoPage created");
  //ObcService::getInstance().subscribe(ChargingInfoPage::ID, std::make_unique<std::function<void(ObcData*)>>( [this](ObcData *data) { obcDataChanged(data); }));
  //obcData = ObcService::getInstance().getData();

  display->requestRender(0);
}

ChargingInfoPage::~ChargingInfoPage() {
  Serial.println("ChargingInfoPage destroyed");
  //ObcService::getInstance().unsubscribe(ChargingInfoPage::ID);
}

//void ChargingInfoPage::obcDataChanged(ObcData *data) {
//  obcData = data;
//  display->requestRender(0);
//}

void ChargingInfoPage::render(uint64_t m) {
  Page::render(m);

  display->prepare();
  display->lcd->cls();

  display->lcd->display(LINES_4);
  unsigned long currentMillis = millis();

  display->lcd->locate(0, 0);
  display->lcd->print("CHARGING");
  uint8_t dots = currentMillis/500 % 4;
  for (int i = 0; i < dots; i++) {
    display->lcd->print('.');
  }

  display->lcd->locate(1, 0);
  display->lcd->write(Display::CURRENT_CHARGE_ICON);
  display->lcd->locate(1, 1);
  //display->lcd->print(obcService->getCurrentVoltage(), 1);
  //display->lcd->print(obcData->maxChargingVoltage, 1);
  display->lcd->print('V');
  display->lcd->locate(1, 9);
  display->lcd->write(Display::MAX_CHARGE_ICON);
  display->lcd->locate(1, 10);
  //display->lcd->print(obcService->getMaxVoltage(), 1);
  display->lcd->print('V');

  display->lcd->locate(2, 1);
  //display->lcd->print(obcService->getCurrentAmperage(), 1);
  display->lcd->print('A');
  display->lcd->locate(2, 10);
  //display->lcd->print(obcService->getMaxAmperage(), 1);
  display->lcd->print('A');

  display->lcd->locate(3, 0);
  display->lcd->write(Display::TEMP_LOW_ICON);
  display->lcd->locate(3, 1);
  //display->lcd->print(obcService->getMinTemp(), 1);
  display->lcd->print('\4');
  display->lcd->locate(3, 9);
  display->lcd->write(Display::TEMP_HIGH_ICON);
  //display->lcd->print(obcService->getMaxTemp(), 1);
  display->lcd->print('\4');

  display->requestRender(100);
}


