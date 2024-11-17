

#ifndef BMS_TEMPERATURES_INFO_PAGE_H
#define BMS_TEMPERATURES_INFO_PAGE_H

#include <SSD1803A_I2C.h>
#include <memory>

#include "../../devices/display.h"
#include "../../pages/page.h"

#include "../../services/bms/bmsService.h"

class BmsTemperaturesInfoPage: public Page {
public:
    BmsTemperaturesInfoPage(Display *display);
    ~BmsTemperaturesInfoPage();
    void render(u_int64_t millis) override;
    void loop() override {};

    static constexpr uint8_t ID = 2;

private:
    BmsTempsData *bmsTempsData;

    void bmsDataChanged(BmsTempsData *bmsTempsData);
};

#endif 