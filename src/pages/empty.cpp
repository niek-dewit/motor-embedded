
#include "page.h"
#include <Arduino.h> 
#include "../devices/display.h"


EmptyPage::EmptyPage() : Page(EmptyPage::ID) {}

void EmptyPage::display(SSD1803A_I2C *lcd) {
  lcd->cls();
  lcd->display(LINES_3_2);

  lcd->locate(1, 0);
  lcd->print("de Wit  Software");

}


