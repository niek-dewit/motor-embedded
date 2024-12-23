#ifndef DISPLAY_H
#define DISPLAY_H

#include <SSD1803A_I2C.h>
#include <memory>

class Display {
public:
    Display(uint8_t i2caddr, u_int8_t resetPin, std::unique_ptr<std::function<void(u_int64_t)>> requestRenderCallback); 
    void prepare() const;
    void requestRender(u_int64_t scheduledAfterMillis) const;
    void init();

   
    static const uint8_t CURRENT_CHARGE_ICON = 0;
    static const uint8_t MAX_CHARGE_ICON = 1;
    static const uint8_t TEMP_LOW_ICON = 2;
    static const uint8_t TEMP_HIGH_ICON = 3;
    static const uint8_t CELSIUS_ICON = 4;

    std::unique_ptr<SSD1803A_I2C> lcd;
private:

    uint8_t i2caddr;
    uint8_t resetPin;
    std::unique_ptr<std::function<void(u_int64_t)>> requestRenderCallback;
};

#endif 