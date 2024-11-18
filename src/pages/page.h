

#ifndef PAGE_H
#define PAGE_H

#include <SSD1803A_I2C.h>
#include <memory>
#include "../devices/display.h"

class Page {
public:
    Page() {};
    virtual void init(uint64_t id, const Display *display) {
        this->display = display;
        this->pageId = id;
        this->initialized = true;
        
        display->prepare();
        display->lcd->cls();
    }
    virtual void render(u_int64_t m) {
        lastRendered = m;
    };
    virtual void loop() {};
    
    virtual ~Page() {}
    u_int64_t lastRendered = 0;
    const Display *display = nullptr;
    uint8_t pageId = 0;
    bool initialized = false;

private:


};



#endif 