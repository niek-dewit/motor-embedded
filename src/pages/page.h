

#ifndef PAGE_H
#define PAGE_H

#include <SSD1803A_I2C.h>
#include <memory>
#include "../devices/display.h"

class Page {
public:
    Page(uint8_t id, Display *display): pageId(id), display(display) {};
    virtual void render(u_int64_t m) {
        lastRendered = m;
    };
    virtual void loop() {};
    
    uint8_t pageId;
    virtual ~Page() {}
    u_int64_t lastRendered = 0;
protected:
    Display *display;

};



#endif 