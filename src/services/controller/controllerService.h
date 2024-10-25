#ifndef ControllerService_H
#define ControllerService_H
#include <Arduino.h> 
#include "../canBus/canBusService.h"
#include "controllerDataTypes.h"

class ControllerService {
public:
    ControllerService();
    static const u_int32_t COMPONENT_STATUS_MESSAGE_ID = (u_int32_t)0x0CF11E05;
    static const u_int32_t INPUT_STATUS_MESSAGE_ID = (u_int32_t)0x0CF11F05;
private:

    void handleInputStatusMessage(const CAN_message_t &msg);
    void handleComponentStatusMessage(const CAN_message_t &msg);
};

#endif
