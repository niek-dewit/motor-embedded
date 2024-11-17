
#include "empty.h"
#include <Arduino.h> 


EmptyPage::EmptyPage(Display *display) : Page(EmptyPage::ID, display) {
  Serial.println("EmptyPage created");
  display->requestRender(1000);
}

EmptyPage::~EmptyPage() {
  Serial.println("EmptyPage destroyed");
}

void EmptyPage::render(uint64_t m) {
  Page::render(m);

  display->prepare();
  display->lcd->cls();
  display->lcd->display(LINES_3_2);

  display->lcd->locate(1, 0);
  display->lcd->print("de Wit  Software");
  display->lcd->locate(2, 0);
  display->lcd->print(millis(), DEC);

  display->requestRender(1000);
}


