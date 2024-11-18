

#ifndef DCDC_INFO_PAGE_H
#define DCDC_INFO_PAGE_H

#include <SSD1803A_I2C.h>
#include <memory>

#include "../../devices/display.h"
#include "../../pages/page.h"

#include "../../services/dcdc/dcdcService.h"


class DcdcInfoPage: public Page {
public:
    DcdcInfoPage() {};
    ~DcdcInfoPage();
    void render(u_int64_t millis) override;
    void loop() override {};
    void init(uint64_t id, const Display *display) override;


private:
    DcdcComponentStatusData *dcdcComponentStatusData;

    void dcdcComponentStatusDataChanged(DcdcComponentStatusData *dcdcComponentStatusData);
    const char* getStatusString();

    static const u_int16_t STATUS_CYCLE_TIME = 1000;

    enum StatusIndex {
        overTemperature = 0,
        overTemperatureProtection,
        inputOverVoltage,
        inputUnderVoltage,
        outputOverVoltage,
        outputUnderVoltage,
        outputOverCurrent,
        ready,
        statusWorking,
        hardwareFault,
        canCommunicationFault,
        fanOn,
        shutOffFault,
        waterFanOn,
        hvilFault
    };
    static constexpr const char* const STATUS_MESSAGES[] = {
        "Over T.",
        "T.Protect",
        "Over Vin",
        "Under Vin",
        "Over Vout",
        "UnderVout",
        "Over Aout",
        "Ready",
        "Working",
        "Hw. Flt",
        "Comm. Flt",
        "Fan on",
        "Shut. Flt",
        "Water on",
        "HVIL. Flt"
    };

};
#endif 