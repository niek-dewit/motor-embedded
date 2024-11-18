#include "buttonsService.h"
#include <Arduino.h> 


ButtonsService::ButtonsService() : buttons() {
  registerButton(ButtonPin::BUTTON1);
  registerButton(ButtonPin::BUTTON2);
  registerButton(ButtonPin::BUTTON3);
  registerButton(ButtonPin::BUTTON4);
  registerButton(ButtonPin::BUTTON5);
  initialized = true;
}

void ButtonsService::registerButton(ButtonPin signalPin) {
  buttons[signalPin] = std::make_unique<Button>((uint8_t)signalPin);
}

Button& ButtonsService::getButton(ButtonPin signalPin) {
  return *buttons.find(signalPin)->second;
}

void ButtonsService::loop() {

  if(!initialized) {
    return;
  }

  for (auto& button : buttons) {
    button.second->update();
  }

}
