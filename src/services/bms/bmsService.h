#ifndef BmsService_H
#define BmsService_H
#include <Arduino.h> 
#include "../canBus/canBusService.h"
#include "bmsDataTypes.h"

class BmsService {
public:
    BmsService(); 
    static const u_int8_t bmsId = 0x45;
    static const u_int32_t BMS_TEMPS_MESSAGE_ID = ((CAN_PACKET_BMS_TEMPS << 8) | BmsService::bmsId);
    static const u_int32_t BMS_V_TOT_MESSAGE_ID = ((CAN_PACKET_BMS_V_TOT << 8) | BmsService::bmsId);
    static const u_int32_t BMS_I_MESSAGE_ID = ((CAN_PACKET_BMS_I << 8) | BmsService::bmsId);
    static const u_int32_t BMS_AH_WH_MESSAGE_ID = ((CAN_PACKET_BMS_AH_WH << 8) | BmsService::bmsId);
    static const u_int32_t BMS_HUM_MESSAGE_ID = ((CAN_PACKET_BMS_HUM << 8) | BmsService::bmsId);
    static const u_int32_t BMS_SOC_SOH_TEMP_STAT_MESSAGE_ID = ((CAN_PACKET_BMS_SOC_SOH_TEMP_STAT << 8) | BmsService::bmsId);
    static const u_int32_t BMS_AH_WH_CHG_TOTAL_MESSAGE_ID = ((CAN_PACKET_BMS_AH_WH_CHG_TOTAL << 8) | BmsService::bmsId);
    static const u_int32_t BMS_AH_WH_DIS_TOTAL_MESSAGE_ID = ((CAN_PACKET_BMS_AH_WH_DIS_TOTAL << 8) | BmsService::bmsId);
    static const u_int32_t BMS_V_CELL_MESSAGE_ID = ((CAN_PACKET_BMS_V_CELL << 8) | BmsService::bmsId);
    static const u_int32_t PONG_MESSAGE_ID = ((CAN_PACKET_PONG << 8) | BmsService::bmsId);
    static const u_int32_t SET_DUTY_MESSAGE_ID = ((CAN_PACKET_SET_DUTY << 8) | BmsService::bmsId);
    static const u_int32_t SET_CURRENT_MESSAGE_ID = ((CAN_PACKET_SET_CURRENT << 8) | BmsService::bmsId);
    static const u_int32_t SET_CURRENT_BRAKE_MESSAGE_ID = ((CAN_PACKET_SET_CURRENT_BRAKE << 8) | BmsService::bmsId);
    static const u_int32_t SET_RPM_MESSAGE_ID = ((CAN_PACKET_SET_RPM << 8) | BmsService::bmsId);
    static const u_int32_t SET_POS_MESSAGE_ID = ((CAN_PACKET_SET_POS << 8) | BmsService::bmsId);
    static const u_int32_t BMS_BAL_MESSAGE_ID = ((CAN_PACKET_BMS_BAL << 8) | BmsService::bmsId);
private:

    void handleBmsTempsMessage(const CAN_message_t &msg);
    void handleBmsVtotMessage(const CAN_message_t &msg);
    void handleBmsIMessage(const CAN_message_t &msg);
    void handleBmsAhWhMessage(const CAN_message_t &msg);
    void handleBmsHumMessage(const CAN_message_t &msg);
    void handleBmsSocSohTempStatMessage(const CAN_message_t &msg);
    void handleBmsAhWhChgTotalMessage(const CAN_message_t &msg);
    void handleBmsAhWhDisTotalMessage(const CAN_message_t &msg);
    void handleBmsVCellMessage(const CAN_message_t &msg);
    void handlePongMessage(const CAN_message_t &msg);
    void handleSetDutyMessage(const CAN_message_t &msg);
    void handleSetCurrentMessage(const CAN_message_t &msg);
    void handleSetCurrentBrakeMessage(const CAN_message_t &msg);
    void handleSetRpmMessage(const CAN_message_t &msg);
    void handleSetPosMessage(const CAN_message_t &msg);
    void handleBmsBalMessage(const CAN_message_t &msg);
};

#endif
