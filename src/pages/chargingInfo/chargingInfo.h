

#ifndef CHARGING_INFO_PAGE_H
#define CHARGING_INFO_PAGE_H

#include <SSD1803A_I2C.h>
#include <memory>

#include "../../devices/display.h"
#include "../../pages/page.h"

#include "../../services/obc/obcService.h"


class ChargingInfoPage: public Page {
public:
    ChargingInfoPage() {};
    ~ChargingInfoPage();
    void render(u_int64_t millis) override;
    void loop() override {};
    void init(uint64_t id, const Display *display) override;


private:
    //ObcData *obcData;

    //void obcDataChanged(ObcData *data);
};



#endif 