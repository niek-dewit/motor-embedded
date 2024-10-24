
#include "page.h"
#include <Arduino.h> 
#include "../devices/display.h"

ChargingInfoPage::ChargingInfoPage(ChargingService *chargingService) :
  Page(ChargingInfoPage::ID),
  chargingService(chargingService) {}

void ChargingInfoPage::display(SSD1803A_I2C *lcd) {
  lcd->cls();

  lcd->display(LINES_4);
  unsigned long currentMillis = millis();

  lcd->locate(0, 0);
  lcd->print("CHARGING");
  uint8_t dots = currentMillis/500 % 4;
  for (int i = 0; i < dots; i++) {
    lcd->print('.');
  }

  lcd->locate(1, 0);
  lcd->write(Display::CURRENT_CHARGE_ICON);
  lcd->locate(1, 1);
  lcd->print(chargingService->getCurrentVoltage(), 1);
  lcd->print('V');
  lcd->locate(1, 9);
  lcd->write(Display::MAX_CHARGE_ICON);
  lcd->locate(1, 10);
  lcd->print(chargingService->getMaxVoltage(), 1);
  lcd->print('V');

  lcd->locate(2, 1);
  lcd->print(chargingService->getCurrentAmperage(), 1);
  lcd->print('A');
  lcd->locate(2, 10);
  lcd->print(chargingService->getMaxAmperage(), 1);
  lcd->print('A');

  lcd->locate(3, 0);
  lcd->write(Display::TEMP_LOW_ICON);
  lcd->locate(3, 1);
  lcd->print(chargingService->getMinTemp(), 1);
  lcd->print('\4');
  lcd->locate(3, 9);
  lcd->write(Display::TEMP_HIGH_ICON);
  lcd->print(chargingService->getMaxTemp(), 1);
  lcd->print('\4');
}


