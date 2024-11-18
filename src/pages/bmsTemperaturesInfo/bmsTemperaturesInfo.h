

#ifndef BMS_TEMPERATURES_INFO_PAGE_H
#define BMS_TEMPERATURES_INFO_PAGE_H

#include <SSD1803A_I2C.h>
#include <memory>

#include "../../devices/display.h"
#include "../../pages/page.h"

#include "../../services/bms/bmsService.h"

class BmsTemperaturesInfoPage: public Page {
public:
    BmsTemperaturesInfoPage() {};
    ~BmsTemperaturesInfoPage();
    void render(u_int64_t millis) override;
    void loop() override {};
    void init(uint64_t id, const Display *display) override;

private:
    BmsTempsData *bmsTempsData;

    void bmsDataChanged(BmsTempsData *bmsTempsData);
};

#endif 