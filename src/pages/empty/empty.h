

#ifndef EMPTY_PAGE_H
#define EMPTY_PAGE_H

#include <SSD1803A_I2C.h>
#include <memory>

#include "../../devices/display.h"
#include "../../pages/page.h"


class EmptyPage: public Page {  
  
public:
    EmptyPage() {};  
    ~EmptyPage();
    void render(u_int64_t millis) override;
    void loop() override {};
    void init(uint64_t id, const Display *display) override;
};


#endif 