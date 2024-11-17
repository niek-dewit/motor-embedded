#ifndef INTERFACESERVICE_H
#define INTERFACESERVICE_H

#include "../obc/obcService.h"
#include <SSD1803A_I2C.h>
#include <map>
#include "../../devices/display.h"
#include "../../pages/page.h"
#include <memory>

class InterfaceService {
public:
    static InterfaceService& getInstance() {
        static InterfaceService instance;
        return instance;
    }

    static const u_int8_t RENDER_DEBOUNCE_TIME = 10;
    static const u_int16_t DISPLAY_REINITIALZE_DELAY = 60000;
    void loop();
    bool logging = true;


private:

    InterfaceService();  

    std::map<u_int8_t, std::unique_ptr<Display>> displays;
    std::map<u_int8_t, std::unique_ptr<Page>> pages;
    std::map<u_int8_t, u_int64_t> pendingRefreshes;
    bool initialized = false;
    int64_t lastDisplayInitializeMillis = 0;

    unsigned long previousMillis = 0;
    void registerDisplay(u_int8_t display, u_int8_t resetPin);
    void registerPage(u_int8_t pageId, std::unique_ptr<Page> page);
    void refreshRequested(u_int8_t displayId, u_int64_t scheduledAfterMillis);
    void checkPendingRefreshes(u_int8_t displayId);
    void reinitializeDisplays();
};

#endif 



