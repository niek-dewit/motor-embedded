

#ifndef CHARGING_PORT_INFO_PAGE_H
#define CHARGING_PORT_INFO_PAGE_H

#include <SSD1803A_I2C.h>
#include <memory>

#include "../../devices/display.h"
#include "../../pages/page.h"

#include "../../services/obc/obcService.h"


class ChargingPortInfoPage: public Page {
public:
    ChargingPortInfoPage() {};
    ~ChargingPortInfoPage();
    void render(u_int64_t millis) override;
    void loop() override {};
    void init(uint64_t id, const Display *display) override;


private:
    ObcStatusData *obcStatusData;

    void obcStatusDataChanged(ObcStatusData *obcStatusData);
    const char* getStatusString();

    static const u_int16_t STATUS_CYCLE_TIME = 1000;

    enum StatusIndex {
        socketOverheating = 0,
        electronicLockPending,
        electronicLockLocked,
        electronicLockUnlocked,
        electronicLockUnlockFault,
        electronicLockLockedFault,
        s2SwitchClosed,
    };

    static constexpr const char* const STATUS_MESSAGES[] = {
        "Socket overheat",
        "Lock Pending",
        "Lock locked",
        "Lock unlocked",
        "Lock unlock flt.",
        "Lock lock flt.",
        "S2 switch closed",
    };

};
#endif 