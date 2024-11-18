#ifndef ButtonsService_H
#define ButtonsService_H
#include <Arduino.h>
#include <map>
#include <memory>
#include "../../devices/button.h"


class ButtonsService {
 public:
  static ButtonsService &getInstance() {
    static ButtonsService instance;
    return instance;
  }
  enum class ButtonPin {
      BUTTON1 = 9,
      BUTTON2 = 8,
      BUTTON3 = 7,
      BUTTON4 = 6,
      BUTTON5 = 5
  };

  Button& getButton(ButtonPin id);
  void loop();

 private:
  ButtonsService();
  bool initialized = false;

  void registerButton(ButtonPin signalPin);
  std::map<ButtonPin, std::unique_ptr<Button>> buttons;
};

#endif
