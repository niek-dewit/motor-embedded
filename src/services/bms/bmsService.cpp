#include "bmsService.h"

#include <Arduino.h>
#include <libBuffer.h>

#include "../bms/bmsDataTypes.h"

BmsService::BmsService() {
  CanBusService::getInstance().registerHandler(BmsService::BMS_TEMPS_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>([this](const CAN_message_t &msg) { handleBmsTempsMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::BMS_V_TOT_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>([this](const CAN_message_t &msg) { handleBmsVtotMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::BMS_I_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>([this](const CAN_message_t &msg) { handleBmsIMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::BMS_AH_WH_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>([this](const CAN_message_t &msg) { handleBmsAhWhMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::BMS_HUM_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>([this](const CAN_message_t &msg) { handleBmsHumMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::BMS_SOC_SOH_TEMP_STAT_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>([this](const CAN_message_t &msg) { handleBmsSocSohTempStatMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::BMS_AH_WH_CHG_TOTAL_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>([this](const CAN_message_t &msg) { handleBmsAhWhChgTotalMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::BMS_AH_WH_DIS_TOTAL_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>([this](const CAN_message_t &msg) { handleBmsAhWhDisTotalMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::BMS_V_CELL_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>([this](const CAN_message_t &msg) { handleBmsVCellMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::PONG_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>([this](const CAN_message_t &msg) { handlePongMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::SET_DUTY_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>([this](const CAN_message_t &msg) { handleSetDutyMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::SET_CURRENT_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>([this](const CAN_message_t &msg) { handleSetCurrentMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::SET_CURRENT_BRAKE_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>([this](const CAN_message_t &msg) { handleSetCurrentBrakeMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::SET_RPM_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>([this](const CAN_message_t &msg) { handleSetRpmMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::SET_POS_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>([this](const CAN_message_t &msg) { handleSetPosMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::BMS_BAL_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>([this](const CAN_message_t &msg) { handleBmsBalMessage(msg); }));
}

void BmsService::handleBmsTempsMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::BMS_TEMPS_MESSAGE_ID) {
    int32_t get_index = 0;

    u_int8_t sensorPointer = libBufferGet_uint8(msg.buf, &get_index);
    u_int8_t temperaturesCount = libBufferGet_uint8(msg.buf, &get_index);

    BmsTempsData *tempsData = bmsTempsObservable.getData();
    for (uint8_t i = 0; i < 3; i++) {
      u_int8_t temperatureIndex = sensorPointer + ((get_index - 2) / 2);
      if (temperatureIndex < temperaturesCount) {
        float temperature = libBufferGet_float16(msg.buf, 1e2, &get_index);
        tempsData->temperaturesMap[temperatureIndex] = temperature;
      }
    }
    bmsTempsObservable.notifyListeners();
  }
}

void BmsService::handleBmsVtotMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::BMS_V_TOT_MESSAGE_ID) {
    int32_t get_index = 0;
    float packVoltage = libBufferGet_float32_auto(msg.buf, &get_index);
    float chargerVoltage = libBufferGet_float32_auto(msg.buf, &get_index);

    BmsVtotData *bmsVtotData = bmsVtotObservable.getData();
    bmsVtotData->packVoltage = packVoltage;
    bmsVtotData->chargerVoltage = chargerVoltage;
    bmsVtotObservable.notifyListeners();
  }
}
void BmsService::handleBmsIMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::BMS_I_MESSAGE_ID) {
    int32_t get_index = 0;
    float packCurrent = libBufferGet_float32_auto(msg.buf, &get_index);

    BmsIData *bmsIData = bmsIObservable.getData();
    bmsIData->packCurrent = packCurrent;
    bmsIObservable.notifyListeners();
  }
}
void BmsService::handleBmsAhWhMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::BMS_AH_WH_MESSAGE_ID) {
    int32_t get_index = 0;
    float packAh = libBufferGet_float32_auto(msg.buf, &get_index);  // probably not implemented
    float packWh = libBufferGet_float32_auto(msg.buf, &get_index);  // probably not implemented

    BmsAhWhData *bmsAhWhData = bmsAhWhObservable.getData();
    bmsAhWhData->packAh = packAh;
    bmsAhWhData->packWh = packWh;
    bmsAhWhObservable.notifyListeners();
  }
}
void BmsService::handleBmsHumMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::BMS_HUM_MESSAGE_ID) {
    int32_t get_index = 0;

    float temperatures0 = libBufferGet_float16(msg.buf, 1e2, &get_index);  // probably bms circuit board temperature?
    float humidity = libBufferGet_float16(msg.buf, 1e2, &get_index);       // no sensor connected
    float temperatures1 = libBufferGet_float16(msg.buf, 1e2, &get_index);  // -50, unused?

    BmsHumData *bmsHumData = bmsHumObservable.getData();
    bmsHumData->temperatures0 = temperatures0;
    bmsHumData->humidity = humidity;
    bmsHumData->temperatures1 = temperatures1;
    bmsHumObservable.notifyListeners();
  }
}
void BmsService::handleBmsSocSohTempStatMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::BMS_SOC_SOH_TEMP_STAT_MESSAGE_ID) {
    int32_t get_index = 0;

    float cellVoltageLow = libBufferGet_float16(msg.buf, 1e3, &get_index);   // mV
    float cellVoltageHigh = libBufferGet_float16(msg.buf, 1e3, &get_index);  // mV

    u_int8_t currentSoC = libBufferGet_uint8(msg.buf, &get_index);
    u_int8_t maxSoC = libBufferGet_uint8(msg.buf, &get_index);
    u_int8_t tempBatteryHigh = libBufferGet_uint8(msg.buf, &get_index);
    u_int8_t state = libBufferGet_uint8(msg.buf, &get_index);

    bool chargeDesired = state & 0b00000001;
    bool balanceActive = state >> 1 & 0b00000001;
    bool chargeAllowed = state >> 2 & 0b00000001;

    BmsSocSohTempStatData *bmsSocSohTempStatData = bmsSocSohTempStatObservable.getData();
    bmsSocSohTempStatData->cellVoltageLow = cellVoltageLow;
    bmsSocSohTempStatData->cellVoltageHigh = cellVoltageHigh;
    bmsSocSohTempStatData->currentSoC = currentSoC;
    bmsSocSohTempStatData->maxSoC = maxSoC;
    bmsSocSohTempStatData->tempBatteryHigh = tempBatteryHigh;
    bmsSocSohTempStatData->chargeDesired = chargeDesired;
    bmsSocSohTempStatData->balanceActive = balanceActive;
    bmsSocSohTempStatData->chargeAllowed = chargeAllowed;
    bmsSocSohTempStatObservable.notifyListeners();
  }
}
void BmsService::handleBmsAhWhChgTotalMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::BMS_AH_WH_CHG_TOTAL_MESSAGE_ID) {
    // cannot find firmware implementation, probably empty and not needed
    bmsAhWhChgTotalObservable.notifyListeners();
  }
}
void BmsService::handleBmsAhWhDisTotalMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::BMS_AH_WH_DIS_TOTAL_MESSAGE_ID) {
    // cannot find firmware implementation, probably empty and not needed
    bmsAhWhDisTotalObservable.notifyListeners();
  }
}
void BmsService::handleBmsVCellMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::BMS_V_CELL_MESSAGE_ID) {
    int32_t get_index = 0;

    u_int8_t cellPointer = libBufferGet_uint8(msg.buf, &get_index);
    u_int8_t cellCount = libBufferGet_uint8(msg.buf, &get_index);

    BmsVCellData *bmsVCellData = bmsVCellObservable.getData();

    for (uint8_t i = 0; i < 3; i++) {
      u_int8_t cellIndex = cellPointer + ((get_index - 2) / 2);
      if (cellIndex < cellCount) {
        float voltage = libBufferGet_float16(msg.buf, 1e3, &get_index);
        bmsVCellData->cellVoltagesMap[cellIndex] = voltage;
      }
    }

    bmsVCellObservable.notifyListeners();
  }
}
void BmsService::handlePongMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::PONG_MESSAGE_ID) {
    int32_t get_index = 0;

    u_int8_t canId = libBufferGet_uint8(msg.buf, &get_index);
    u_int8_t hwType = libBufferGet_uint8(msg.buf, &get_index);

    BmsPongData *pongData = pongObservable.getData();
    pongData->pongReceivedMillis = millis();
    pongData->pongCanId = canId;
    pongData->pongHwType = hwType;
    pongObservable.notifyListeners();
  }
}
void BmsService::handleSetDutyMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::SET_DUTY_MESSAGE_ID) {
    int32_t get_index = 0;
    float duty = (float)libBufferGet_int32(msg.buf, &get_index) / 100000.0f;

    BmsSetDutyData *setDutyData = setDutyObservable.getData();
    setDutyData->setEscDuty = duty;
    setDutyObservable.notifyListeners();
  }
}
void BmsService::handleSetCurrentMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::SET_CURRENT_MESSAGE_ID) {
    int32_t get_index = 0;
    float current = (float)libBufferGet_int32(msg.buf, &get_index) / 1000.0f;

    BmsSetCurrentData *setCurrentData = setCurrentObservable.getData();
    setCurrentData->setEscCurrent = current;
    setCurrentObservable.notifyListeners();
  }
}
void BmsService::handleSetCurrentBrakeMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::SET_CURRENT_BRAKE_MESSAGE_ID) {
    int32_t get_index = 0;
    float brakeCurrent = (float)libBufferGet_int32(msg.buf, &get_index) / 1000.0f;

    BmsSetCurrentBrakeData *setCurrentBrakeData = setCurrentBrakeObservable.getData();
    setCurrentBrakeData->setEscBrakeCurrent = brakeCurrent;
    setCurrentBrakeObservable.notifyListeners();
  }
}
void BmsService::handleSetRpmMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::SET_RPM_MESSAGE_ID) {
    int32_t get_index = 0;
    int32_t rpm = libBufferGet_int32(msg.buf, &get_index);

    BmsSetRpmData *setRpmData = setRpmObservable.getData();
    setRpmData->setEscRpm = rpm;
    setRpmObservable.notifyListeners();
  }
}
void BmsService::handleSetPosMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::SET_POS_MESSAGE_ID) {
    int32_t get_index = 0;
    float pos = (float)libBufferGet_int32(msg.buf, &get_index) / 1000000.0f;

    BmsSetPosData *setPosData = setPosObservable.getData();
    setPosData->setEscPos = pos;
    setPosObservable.notifyListeners();
  }
}
void BmsService::handleBmsBalMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::BMS_BAL_MESSAGE_ID) {
    int32_t get_index = 0;

    u_int8_t cellCount = libBufferGet_uint8(msg.buf, &get_index);

    uint64_t bal_state =
      (uint64_t)libBufferGet_uint8(msg.buf, &get_index) << 48 |
      (uint64_t)libBufferGet_uint8(msg.buf, &get_index) << 40 |
      (uint64_t)libBufferGet_uint8(msg.buf, &get_index) << 32 |
      (uint64_t)libBufferGet_uint8(msg.buf, &get_index) << 24 |
      (uint64_t)libBufferGet_uint8(msg.buf, &get_index) << 16 |
      (uint64_t)libBufferGet_uint8(msg.buf, &get_index) << 8 |
      (uint64_t)libBufferGet_uint8(msg.buf, &get_index) << 0;

    BmsBalData *balData = bmsBalObservable.getData();

    for (uint8_t cellPointer = 0; cellPointer < cellCount; cellPointer++) {
      bool balancing = (bal_state >> cellPointer) & 0b1;
      balData->cellBalancingMap[cellPointer] = balancing;
    }

    bmsBalObservable.notifyListeners();
  }
}

void BmsService::printData() {
  BmsBalData *bmsBalData = bmsBalObservable.getData();
  bmsBalData->printBalancingStatus();

  BmsVCellData *bmsVCellData = bmsVCellObservable.getData();
  bmsVCellData->printVoltagesStatus();

  BmsTempsData *bmsTempsData = bmsTempsObservable.getData();
  bmsTempsData->printTemperaturesStatus();

  BmsSetDutyData *setDutyData = setDutyObservable.getData();
  Serial.print("Set ESC Duty: ");
  Serial.println(setDutyData->setEscDuty);

  BmsSetCurrentData *setCurrentData = setCurrentObservable.getData();
  Serial.print("Set ESC Current: ");
  Serial.println(setCurrentData->setEscCurrent);

  BmsSetCurrentBrakeData *setCurrentBrakeData = setCurrentBrakeObservable.getData();
  Serial.print("Set ESC Brake Current: ");
  Serial.println(setCurrentBrakeData->setEscBrakeCurrent);

  BmsSetRpmData *setRpmData = setRpmObservable.getData();
  Serial.print("Set ESC RPM: ");
  Serial.println(setRpmData->setEscRpm);

  BmsSetPosData *setPosData = setPosObservable.getData();
  Serial.print("Set ESC Position: ");
  Serial.println(setPosData->setEscPos);

  BmsPongData *pongData = pongObservable.getData();
  Serial.print("Pong received on: ");
  Serial.println(pongData->pongReceivedMillis);
  Serial.print("Pong CanId: ");
  Serial.print(pongData->pongCanId);
  Serial.print(" - Pong HwType: ");
  Serial.println(pongData->pongHwType);

  BmsSocSohTempStatData *bmsSocSohTempStatData = bmsSocSohTempStatObservable.getData();
  Serial.print("Vlow: ");
  Serial.print(bmsSocSohTempStatData->cellVoltageLow);
  Serial.print("V - Vhigh: ");
  Serial.print(bmsSocSohTempStatData->cellVoltageHigh);
  Serial.println("V");
  Serial.print("SoC: ");
  Serial.print(bmsSocSohTempStatData->currentSoC);
  Serial.print(" / ");
  Serial.println(bmsSocSohTempStatData->maxSoC);
  Serial.print("Temperature high: ");
  Serial.print(bmsSocSohTempStatData->tempBatteryHigh);
  Serial.println("°C");
  Serial.print("Charge desired: ");
  Serial.print(bmsSocSohTempStatData->chargeDesired);
  Serial.print(" - Balance active: ");
  Serial.print(bmsSocSohTempStatData->balanceActive);
  Serial.print(" - Charge allowed: ");
  Serial.println(bmsSocSohTempStatData->chargeAllowed);

  BmsHumData *bmsHumData = bmsHumObservable.getData();
  Serial.print("Temperature0: ");
  Serial.print(bmsHumData->temperatures0);
  Serial.print("Temperature1: ");
  Serial.println(bmsHumData->temperatures1);
  Serial.print("Humidity: ");
  Serial.println(bmsHumData->humidity);

  BmsAhWhData *bmsAhWhData = bmsAhWhObservable.getData();
  Serial.print("Capacity: ");
  Serial.print(bmsAhWhData->packAh);
  Serial.print("Ah - ");
  Serial.print(bmsAhWhData->packWh);
  Serial.println("Wh");

  BmsIData *bmsIData = bmsIObservable.getData();
  Serial.print("I: ");
  Serial.print(bmsIData->packCurrent);
  Serial.println("A");

  BmsVtotData *bmsVtotData = bmsVtotObservable.getData();
  Serial.print("Vtot. Pack: ");
  Serial.print(bmsVtotData->packVoltage);
  Serial.print("V - ");
  Serial.print("Vtot. Charger: ");
  Serial.print(bmsVtotData->chargerVoltage);
  Serial.println("V");
}