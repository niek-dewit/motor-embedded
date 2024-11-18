#ifndef HEARTBEAT_SERVICE_H
#define HEARTBEAT_SERVICE_H

#include "../obc/obcService.h"
#include <SSD1803A_I2C.h>
#include <map>
#include "../../devices/display.h"
#include "../../pages/page.h"
#include <memory>

class HeartbeatService {
public:
    static HeartbeatService& getInstance() {
        static HeartbeatService instance;
        return instance;
    }
    
    void loop();


private:
    HeartbeatService();  
    u_int64_t heartBeatInterval = 1000;
    int prevHeartBeat = 0;
    bool blinking = false;

};

#endif 



