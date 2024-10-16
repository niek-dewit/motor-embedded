#ifndef INTERFACESERVICE_H
#define INTERFACESERVICE_H

#include "chargingService.h"
#include <SSD1803A_I2C.h>
#include <map>
#include "../devices/display.h"
#include "../pages/page.h"
#include <memory>

class InterfaceService {
public:
    InterfaceService(ChargingService *chargingService);  
    static const u_int8_t PAGE_CHARGING_INFO = 0;
    static const u_int8_t PAGE_DISCHARGING_INFO = 1;

    void loop();


private:
    std::map<u_int8_t, std::unique_ptr<Display>> displays;
    std::map<u_int8_t, std::unique_ptr<Page>> pages;

    unsigned long previousMillis = 0;
    void registerDisplay(u_int8_t display, u_int8_t resetPin);
    void registerPage(u_int8_t pageId, std::unique_ptr<Page> page);
    void refresh();
};

#endif 



