#ifndef DISPLAYS_SERVICE_H
#define DISPLAYS_SERVICE_H

#include "../obc/obcService.h"
#include <SSD1803A_I2C.h>
#include <map>
#include "../../devices/display.h"
#include "../../pages/page.h"
#include <memory>

class DisplaysService {
public:
    static DisplaysService& getInstance() {
        static DisplaysService instance;
        return instance;
    }
    enum class DisplayAddress {
        DISPLAY1 = 0x3C,
        DISPLAY2 = 0x3D
    };

    enum class DisplayResetPin {
        DISPLAY1 = 3,
        DISPLAY2 = 4
    };


    static const u_int8_t RENDER_DEBOUNCE_TIME = 10;
    static const u_int16_t DISPLAY_REINITIALZE_DELAY = 60000;
    
    const Display* getDisplay(DisplayAddress id);
    void registerPage(DisplayAddress displayId, std::unique_ptr<Page> page);
    void setReadyCallback(std::unique_ptr<std::function<void()>> readyCallback);

    
    void loop();
    bool logging = true;



private:
    DisplaysService();  
    bool initialized = false;
    int64_t lastPageId = 0;
    std::map<DisplayAddress, std::unique_ptr<Display>> displays;
    std::map<DisplayAddress, std::unique_ptr<Page>> pages;
    std::map<DisplayAddress, u_int64_t> pendingRefreshes;
    std::unique_ptr<std::function<void()>> readyCallback;

    int64_t lastDisplayInitializeMillis = 0;

    unsigned long previousMillis = 0;
    void init();
    void registerDisplay(DisplayAddress displayId, DisplayResetPin resetPin);
    void refreshRequested(DisplayAddress displayId, u_int64_t scheduledAfterMillis);
    void checkPendingRefreshes(DisplayAddress displayId);
    void reinitializeDisplays();
};

#endif 



