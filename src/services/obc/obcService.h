#ifndef ObcService_H
#define ObcService_H
#include "../canBus/canBusService.h"

class ObcService {
public:
    ObcService(); 

    static const u_int32_t CAN_SEND_STATUS_MESSAGE_ID = (u_int32_t)0x18FF50E5;
    static const u_int32_t CAN_RECEIVE_COMMAND_MESSAGE_ID = (u_int32_t)0x1806E5F4;

    float getCurrentVoltage();
    float getMaxVoltage();
    float getCurrentAmperage();
    float getMaxAmperage();
    float getMinTemp();
    float getMaxTemp();

private:
    float currentVoltage;
    float maxVoltage;
    float currentAmperage;
    float maxAmperage;
    float minTemp;
    float maxTemp;
    
    void handleStatusMessage(const CAN_message_t &msg);
    void handleCommandMessage(const CAN_message_t &msg);
};

#endif