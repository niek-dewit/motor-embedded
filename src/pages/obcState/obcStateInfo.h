

#ifndef OBC_STATE_INFO_PAGE_H
#define OBC_STATE_INFO_PAGE_H

#include <SSD1803A_I2C.h>
#include <memory>

#include "../../devices/display.h"
#include "../../pages/page.h"

#include "../../services/obc/obcService.h"


class ObcStateInfoPage: public Page {
public:
    ObcStateInfoPage(Display *display);
    ~ObcStateInfoPage();
    void render(u_int64_t millis) override;
    void loop() override {};

    static constexpr uint8_t ID = 6;

private:
    ObcStatusData *obcStatusData;

    void obcStatusDataChanged(ObcStatusData *obcStatusData);
    const char* getStatusString();

    static const u_int16_t STATUS_CYCLE_TIME = 1000;

    enum StatusIndex {
        hardwareProtection = 0,
        temperatureProtection,
        inputVoltageNormal,
        inputUnderVoltage,
        inputOverVoltage,
        noInputVoltage,
        outputUnderVoltage,
        outputOverVoltage,
        outputOverCurrent,
        outputShortCircuit,
        communicationReceiveTimeout,
        workingStatusUndefined,
        workingStatusNormal,
        workingStatusStop,
        workingStatusStopStandby,
        initializationComplete,
        fanOn,
        coolingPumpOn,
    };

    static constexpr char* const STATUS_MESSAGES[] = {
        "Hw. Flt.",
        "Temp. Flt.",
        "OK Vin",
        "Under Vin",
        "Over Vin",
        "No Vin",
        "Under Vout",
        "Over Vout",
        "Over Aout",
        "ShortedOut",
        "Comm. Flt.",
        "Und. Work",
        "OK Work",
        "Stop Work",
        "Ready Work",
        "OK Init",
        "Fan on",
        "Pump on",
    };

};
#endif 