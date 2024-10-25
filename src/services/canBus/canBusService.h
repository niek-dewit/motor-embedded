#ifndef CanBusService_H
#define CanBusService_H
#include <FlexCAN_T4.h>
#include <memory>
#include <map>

class CanBusService {
public:

    static CanBusService& getInstance() {
        static CanBusService instance;
        return instance;
    }
    static void callCanReceived(const CAN_message_t &msg);
    void registerHandler(uint32_t id, std::unique_ptr<std::function<void(const CAN_message_t &)>> callback);
    void loop();
private:    
    CanBusService(); 

    FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;

    void canReceived(const CAN_message_t &msg);
    void unknownReceived(const CAN_message_t &msg);

    std::map<uint32_t, std::unique_ptr<std::function<void(const CAN_message_t &)>>> handlers;
    std::map<uint32_t, uint32_t> unknownMessagesMap;
};

#endif