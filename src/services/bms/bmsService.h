#ifndef BmsService_H
#define BmsService_H
#include <Arduino.h>

#include <map>

#include "../../observable/observable.h"
#include "../canBus/canBusService.h"
#include "bmsDataTypes.h"
struct BmsTempsData {
  std::map<uint8_t, float> temperaturesMap;

  void printTemperaturesStatus() const {
    for (const auto &[sensorPointer, temperature] : temperaturesMap) {
      Serial.print("Temperature: ");
      Serial.print(sensorPointer);
      Serial.print(" - ");
      Serial.print(temperature);
      Serial.print("°C  ");
      if (sensorPointer != 0 && (sensorPointer + 1) % 3 == 0) {
        Serial.println();
      }
    }
    Serial.println();
  }
};

struct BmsVtotData {

  float packVoltage;
  float chargerVoltage;
};
struct BmsIData {

  float packCurrent;
};
struct BmsAhWhData {

  float packAh;
  float packWh;
};
struct BmsHumData {

  float temperatures0;
  float humidity;
  float temperatures1;
};
struct BmsSocSohTempStatData {
float cellVoltageLow;
  float cellVoltageHigh;
  u_int8_t currentSoC;
  u_int8_t maxSoC;
  u_int8_t tempBatteryHigh;
  bool chargeDesired;
  bool balanceActive;
  bool chargeAllowed;
};
struct BmsAhWhChgTotalData {

};
struct BmsAhWhDisTotalData {

};
struct BmsVCellData {

  std::map<uint8_t, float> cellVoltagesMap;
  void printVoltagesStatus() const {
    for (const auto &[cellPointer, voltage] : cellVoltagesMap) {
      Serial.print("Cell: ");
      Serial.print(cellPointer);
      Serial.print(" - ");
      Serial.print(voltage);
      Serial.print("V ");
      if (cellPointer != 0 && (cellPointer + 1) % 3 == 0) {
        Serial.println();
      }
    }
    Serial.println();
  }
};
struct BmsPongData {

  uint32_t pongReceivedMillis;
  uint8_t pongCanId;
  uint8_t pongHwType;
};
struct BmsSetDutyData {

  float setEscDuty;
};
struct BmsSetCurrentData {

  float setEscCurrent;
};
struct BmsSetCurrentBrakeData {

  float setEscBrakeCurrent;
};
struct BmsSetRpmData {

  int32_t setEscRpm;
};
struct BmsSetPosData {

  int32_t setEscPos;
};
struct BmsBalData {

  std::map<uint8_t, bool> cellBalancingMap;
    void printBalancingStatus() const {
    for (const auto &[cellPointer, balancing] : cellBalancingMap) {
      Serial.print("Cell: ");
      Serial.print(cellPointer);
      Serial.print(" - ");
      Serial.print(balancing);
      Serial.print(" ");
      if (cellPointer != 0 && (cellPointer + 1) % 3 == 0) {
        Serial.println();
      }
    }
    Serial.println();
  }
};


class BmsService {
 public:
  static BmsService &getInstance() {
    static BmsService instance;
    return instance;
  }
  static const u_int8_t bmsId = 0x45;
  static const u_int32_t BMS_TEMPS_MESSAGE_ID =             ((CAN_PACKET_BMS_TEMPS << 8) |             BmsService::bmsId);
  static const u_int32_t BMS_V_TOT_MESSAGE_ID =             ((CAN_PACKET_BMS_V_TOT << 8) |             BmsService::bmsId);
  static const u_int32_t BMS_I_MESSAGE_ID =                 ((CAN_PACKET_BMS_I << 8) |                 BmsService::bmsId);
  static const u_int32_t BMS_AH_WH_MESSAGE_ID =             ((CAN_PACKET_BMS_AH_WH << 8) |             BmsService::bmsId);
  static const u_int32_t BMS_HUM_MESSAGE_ID =               ((CAN_PACKET_BMS_HUM << 8) |               BmsService::bmsId);
  static const u_int32_t BMS_SOC_SOH_TEMP_STAT_MESSAGE_ID = ((CAN_PACKET_BMS_SOC_SOH_TEMP_STAT << 8) | BmsService::bmsId);
  static const u_int32_t BMS_AH_WH_CHG_TOTAL_MESSAGE_ID =   ((CAN_PACKET_BMS_AH_WH_CHG_TOTAL << 8) |   BmsService::bmsId);
  static const u_int32_t BMS_AH_WH_DIS_TOTAL_MESSAGE_ID =   ((CAN_PACKET_BMS_AH_WH_DIS_TOTAL << 8) |   BmsService::bmsId);
  static const u_int32_t BMS_V_CELL_MESSAGE_ID =            ((CAN_PACKET_BMS_V_CELL << 8) |            BmsService::bmsId);
  static const u_int32_t PONG_MESSAGE_ID =                  ((CAN_PACKET_PONG << 8) |                  BmsService::bmsId);
  static const u_int32_t SET_DUTY_MESSAGE_ID =              ((CAN_PACKET_SET_DUTY << 8) |              BmsService::bmsId);
  static const u_int32_t SET_CURRENT_MESSAGE_ID =           ((CAN_PACKET_SET_CURRENT << 8) |           BmsService::bmsId);
  static const u_int32_t SET_CURRENT_BRAKE_MESSAGE_ID =     ((CAN_PACKET_SET_CURRENT_BRAKE << 8) |     BmsService::bmsId);
  static const u_int32_t SET_RPM_MESSAGE_ID =               ((CAN_PACKET_SET_RPM << 8) |               BmsService::bmsId);
  static const u_int32_t SET_POS_MESSAGE_ID =               ((CAN_PACKET_SET_POS << 8) |               BmsService::bmsId);
  static const u_int32_t BMS_BAL_MESSAGE_ID =               ((CAN_PACKET_BMS_BAL << 8) |               BmsService::bmsId);

  void printData();

  Observable<BmsTempsData> bmsTempsObservable;
  Observable<BmsVtotData> bmsVtotObservable;
  Observable<BmsIData> bmsIObservable;
  Observable<BmsAhWhData> bmsAhWhObservable;
  Observable<BmsHumData> bmsHumObservable;
  Observable<BmsSocSohTempStatData> bmsSocSohTempStatObservable;
  Observable<BmsAhWhChgTotalData> bmsAhWhChgTotalObservable;
  Observable<BmsAhWhDisTotalData> bmsAhWhDisTotalObservable;
  Observable<BmsVCellData> bmsVCellObservable;
  Observable<BmsPongData> pongObservable;
  Observable<BmsSetDutyData> setDutyObservable;
  Observable<BmsSetCurrentData> setCurrentObservable;
  Observable<BmsSetCurrentBrakeData> setCurrentBrakeObservable;
  Observable<BmsSetRpmData> setRpmObservable;
  Observable<BmsSetPosData> setPosObservable;
  Observable<BmsBalData> bmsBalObservable;

 private:
  BmsService();

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
