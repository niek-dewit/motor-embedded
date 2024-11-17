

#ifndef CHARGING_INFO_PAGE_H
#define CHARGING_INFO_PAGE_H

#include <SSD1803A_I2C.h>
#include <memory>

#include "../../devices/display.h"
#include "../../pages/page.h"

#include "../../services/obc/obcService.h"


class ChargingInfoPage: public Page {
public:
    ChargingInfoPage(Display *display);
    ~ChargingInfoPage();
    void render(u_int64_t millis) override;
    void loop() override {};

    static constexpr uint8_t ID = 1;

private:
    //ObcData *obcData;

    //void obcDataChanged(ObcData *data);
};



#endif 