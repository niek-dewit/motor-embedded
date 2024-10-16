#ifndef DISPLAY_H
#define DISPLAY_H

#include <SSD1803A_I2C.h>
#include "../pages/page.h"
#include <memory>

class Display {
public:
    Display(uint8_t i2caddr, u_int8_t resetPin, Page *initialPage);  
    void setPage(Page *page);
    void refresh();

    static constexpr uint8_t DISPLAY1 = 0x3C;
    static constexpr uint8_t DISPLAY2 = 0x3D;

    static constexpr uint8_t resetPinDisplay2 = 4;
    static constexpr uint8_t resetPinDisplay1 = 3;

    static const uint8_t CURRENT_CHARGE_ICON = 0;
    static const uint8_t MAX_CHARGE_ICON = 1;
    static const uint8_t TEMP_LOW_ICON = 2;
    static const uint8_t TEMP_HIGH_ICON = 3;
    static const uint8_t CELSIUS_ICON = 4;

    Page *currentPage;
private:
    std::unique_ptr<SSD1803A_I2C> lcd;
};

#endif 