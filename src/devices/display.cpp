
#include "../devices/display.h"
#include <Arduino.h> 
#include <Wire.h>

const uint8_t contrast = 55;

uint8_t currentCharge[8] = {
    0b00011,
    0b00110,
    0b01100,
    0b11111,
    0b00111,
    0b00110,
    0b01100,
    0b11000};

uint8_t maxCharge[8] = {
    0b11111,
    0b11111,
    0b00100,
    0b01110,
    0b11111,
    0b00100,
    0b00100,
    0b00100};

uint8_t tempLow[8] = {
    0b01110,
    0b00100,
    0b00100,
    0b00100,
    0b00000,
    0b01000,
    0b01000,
    0b01100};

uint8_t tempHigh[8] = {
    0b01110,
    0b00100,
    0b00100,
    0b00100,
    0b00000,
    0b01010,
    0b01110,
    0b01010};

uint8_t celsius[8] = {
    0b11100,
    0b10100,
    0b11100,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000};

Display::Display(uint8_t i2caddr, uint8_t resetPin, Page *initialPage) {
    lcd = std::make_unique<SSD1803A_I2C>(i2caddr, resetPin);
    setPage(initialPage);

    lcd->begin(DOGS164);

    lcd->create(CURRENT_CHARGE_ICON, currentCharge);
    lcd->create(MAX_CHARGE_ICON, maxCharge);
    lcd->create(TEMP_HIGH_ICON, tempHigh);
    lcd->create(TEMP_LOW_ICON, tempLow);
    lcd->create(CELSIUS_ICON, celsius);
    lcd->display(CONTRAST, contrast);

    refresh();

}

void Display::setPage(Page *page) {
    currentPage = page;
}


void Display::refresh() {
	Wire.begin(); // Not sure why this is necessary exacly, but it works, and measured execution of this linie takes less than 1 microsecond. Compared to 1000 - 20000 microseconds total.
  currentPage->display(lcd.get());
}


