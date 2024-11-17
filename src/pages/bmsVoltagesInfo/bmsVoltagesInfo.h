

#ifndef BMS_VOLTAGES_INFO_PAGE_H
#define BMS_VOLTAGES_INFO_PAGE_H

#include <SSD1803A_I2C.h>
#include <memory>

#include "../../devices/display.h"
#include "../../pages/page.h"

#include "../../services/bms/bmsService.h"


class BmsVoltagesInfoPage: public Page {
public:
    BmsVoltagesInfoPage(Display *display, uint8_t pageIndex);
    ~BmsVoltagesInfoPage();
    void render(u_int64_t millis) override;
    void loop() override;

    static constexpr uint8_t ID = 1;
    static constexpr uint8_t PAGE_MAX = 3;
    uint8_t pageIndex = 0;
    uint32_t prevPageSwitch = 0;

private:
    BmsVCellData *bmsVCellData;

    void bmsDataChanged(BmsVCellData *bmsVCellData);
};
#endif 