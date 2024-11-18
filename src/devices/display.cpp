
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

Display::Display(uint8_t i2caddr, uint8_t resetPin, std::unique_ptr<std::function<void(u_int64_t)>> cb): i2caddr(i2caddr), resetPin(resetPin), requestRenderCallback(std::move(cb)) {
    init();
}

void Display::init() {
    lcd = std::make_unique<SSD1803A_I2C>(i2caddr, resetPin);

    lcd->begin(DOGS164);

    lcd->create(CURRENT_CHARGE_ICON, currentCharge);
    lcd->create(MAX_CHARGE_ICON, maxCharge);
    lcd->create(TEMP_HIGH_ICON, tempHigh);
    lcd->create(TEMP_LOW_ICON, tempLow);
    lcd->create(CELSIUS_ICON, celsius);
    lcd->display(CONTRAST, contrast);
    
    this->requestRender(0);
}

void Display::prepare() const {
    Wire.begin(); // Not sure why this is necessary exacly, but it works, and measured execution of this line takes less than 1 microsecond. Compared to 1000 - 20000 microseconds total.
}

void Display::requestRender(u_int64_t scheduledAfterMillis) const {
    requestRenderCallback->operator()(scheduledAfterMillis);
}

