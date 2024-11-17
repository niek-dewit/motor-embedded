

#ifndef OBC_INSTRUCTED_INFO_PAGE_H
#define OBC_INSTRUCTED_INFO_PAGE_H

#include <SSD1803A_I2C.h>
#include <memory>

#include "../../devices/display.h"
#include "../../pages/page.h"

#include "../../services/obc/obcService.h"


class ObcInstructedInfoPage: public Page {
public:
    ObcInstructedInfoPage(Display *display);
    ~ObcInstructedInfoPage();
    void render(u_int64_t millis) override;
    void loop() override {};

    static constexpr uint8_t ID = 5;

private:
    ObcCommandData *obcCommandData;

    void obcCommandDataChanged(ObcCommandData *obcCommandData);
    const char* getStatusString();

    static const u_int16_t STATUS_CYCLE_TIME = 1000;

    enum StatusIndex {
        commandStartCharging = 0,
        commandCloseCharger,
        commandSleepCharger,
        commandChargingMode,
        commandHeatingMode,
    };
    static constexpr char* const STATUS_MESSAGES[] = {
        "Start Charging",
        "Close Charger",
        "Sleep Charger",
        "Charging Mode",
        "Heating Mode",
    };

};
#endif 