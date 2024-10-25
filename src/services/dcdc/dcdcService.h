#ifndef DcdcService_H
#define DcdcService_H
#include <Arduino.h> 
#include "../canBus/canBusService.h"

class DcdcService {
public:
    DcdcService();
    static const u_int32_t COMPONENT_STATUS_MESSAGE_ID = (u_int32_t)0x1801D08F;
    static const u_int32_t CONTROL_MESSAGE_ID = (u_int32_t)0x18008FD0;
private:

    void handleComponentStatusMessage(const CAN_message_t &msg);
    void handleControlMessage(const CAN_message_t &msg);
};

#endif
