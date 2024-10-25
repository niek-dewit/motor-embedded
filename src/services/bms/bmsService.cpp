#include "bmsService.h"

#include <Arduino.h>
#include <libBuffer.h>

#include "../bms/bmsDataTypes.h"

BmsService::BmsService() {

  CanBusService::getInstance().registerHandler(BmsService::BMS_TEMPS_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>( [this](const CAN_message_t &msg) { handleBmsTempsMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::BMS_V_TOT_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>( [this](const CAN_message_t &msg) { handleBmsVtotMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::BMS_I_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>( [this](const CAN_message_t &msg) { handleBmsIMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::BMS_AH_WH_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>( [this](const CAN_message_t &msg) { handleBmsAhWhMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::BMS_HUM_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>( [this](const CAN_message_t &msg) { handleBmsHumMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::BMS_SOC_SOH_TEMP_STAT_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>( [this](const CAN_message_t &msg) { handleBmsSocSohTempStatMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::BMS_AH_WH_CHG_TOTAL_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>( [this](const CAN_message_t &msg) { handleBmsAhWhChgTotalMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::BMS_AH_WH_DIS_TOTAL_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>( [this](const CAN_message_t &msg) { handleBmsAhWhDisTotalMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::BMS_V_CELL_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>( [this](const CAN_message_t &msg) { handleBmsVCellMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::PONG_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>( [this](const CAN_message_t &msg) { handlePongMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::SET_DUTY_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>( [this](const CAN_message_t &msg) { handleSetDutyMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::SET_CURRENT_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>( [this](const CAN_message_t &msg) { handleSetCurrentMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::SET_CURRENT_BRAKE_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>( [this](const CAN_message_t &msg) { handleSetCurrentBrakeMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::SET_RPM_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>( [this](const CAN_message_t &msg) { handleSetRpmMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::SET_POS_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>( [this](const CAN_message_t &msg) { handleSetPosMessage(msg); }));
  CanBusService::getInstance().registerHandler(BmsService::BMS_BAL_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>( [this](const CAN_message_t &msg) { handleBmsBalMessage(msg); }));

}

void BmsService::handleBmsTempsMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::BMS_TEMPS_MESSAGE_ID) {
    int32_t get_index = 0;

    u_int8_t sensorPointer = libBufferGet_uint8(msg.buf, &get_index);
    u_int8_t temperaturesCount = libBufferGet_uint8(msg.buf, &get_index);

    for (uint8_t i = 0; i < 3; i++) {
      if (sensorPointer + ((get_index - 2) / 2) < temperaturesCount) {
        Serial.print("Sensor: ");
        Serial.print(sensorPointer + ((get_index - 2) / 2));
        Serial.print(" - ");
        float temperature = libBufferGet_float16(msg.buf, 1e2, &get_index);
        Serial.print(temperature);
        Serial.print("°C ");
      }
    }

    Serial.println();
  }
}

void BmsService::handleBmsVtotMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::BMS_V_TOT_MESSAGE_ID) {
    int32_t get_index = 0;
    float packVoltage = libBufferGet_float32_auto(msg.buf, &get_index);
    float chargerVoltage = libBufferGet_float32_auto(msg.buf, &get_index);
    Serial.print("Vtot. Pack: ");
    Serial.print(packVoltage);
    Serial.print("V - ");
    Serial.print("Vtot. Charger: ");
    Serial.print(chargerVoltage);
    Serial.println("V");
  }
}
void BmsService::handleBmsIMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::BMS_I_MESSAGE_ID) {
    int32_t get_index = 0;
    float packCurrent = libBufferGet_float32_auto(msg.buf, &get_index);
    Serial.print("I: ");
    Serial.print(packCurrent);
    Serial.println("A");
  }
}
void BmsService::handleBmsAhWhMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::BMS_AH_WH_MESSAGE_ID) {
    int32_t get_index = 0;
    float packAh = libBufferGet_float32_auto(msg.buf, &get_index);  // probably not implemented
    float packWh = libBufferGet_float32_auto(msg.buf, &get_index);  // probably not implemented
    Serial.print("Capacity: ");
    Serial.print(packAh);
    Serial.print("Ah - ");
    Serial.print(packWh);
    Serial.println("Wh");
  }
}
void BmsService::handleBmsHumMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::BMS_HUM_MESSAGE_ID) {
    int32_t get_index = 0;

    float temperatures0 = libBufferGet_float16(msg.buf, 1e2, &get_index);  // probably bms circuit board temperature?
    float humidity = libBufferGet_float16(msg.buf, 1e2, &get_index);       // no sensor connected
    float temperatures1 = libBufferGet_float16(msg.buf, 1e2, &get_index);  // -50, unused?

    Serial.print("Temperature0: ");
    Serial.print(temperatures0);
    Serial.print("Temperature1: ");
    Serial.println(temperatures1);
    Serial.print("Humidity: ");
    Serial.println(humidity);
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

    Serial.print("Vlow: ");
    Serial.print(cellVoltageLow);
    Serial.print("V - Vhigh: ");
    Serial.print(cellVoltageHigh);
    Serial.println("V");
    Serial.print("SoC: ");
    Serial.print(currentSoC);
    Serial.print(" / ");
    Serial.println(maxSoC);
    Serial.print("Temperature high: ");
    Serial.print(tempBatteryHigh);
    Serial.println("°C");
    Serial.print("Charge desired: ");
    Serial.print(chargeDesired);
    Serial.print(" - Balance active: ");
    Serial.print(balanceActive);
    Serial.print(" - Charge allowed: ");
    Serial.println(chargeAllowed);
  }
}
void BmsService::handleBmsAhWhChgTotalMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::BMS_AH_WH_CHG_TOTAL_MESSAGE_ID) {
    // cannot find firmware implementation, probably empty and not needed
  }
}
void BmsService::handleBmsAhWhDisTotalMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::BMS_AH_WH_DIS_TOTAL_MESSAGE_ID) {
    // cannot find firmware implementation, probably empty and not needed
  }
}
void BmsService::handleBmsVCellMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::BMS_V_CELL_MESSAGE_ID) {
    int32_t get_index = 0;

    u_int8_t cellPointer = libBufferGet_uint8(msg.buf, &get_index);
    u_int8_t cellCount = libBufferGet_uint8(msg.buf, &get_index);

    for (uint8_t i = 0; i < 3; i++) {
      if (cellPointer + ((get_index - 2) / 2) < cellCount) {
        Serial.print("Cell: ");
        Serial.print(cellPointer + ((get_index - 2) / 2));
        Serial.print(" - ");
        float voltage = libBufferGet_float16(msg.buf, 1e3, &get_index);
        Serial.print(voltage);
        Serial.print("V ");
      }
    }
    Serial.println();
  }
}
void BmsService::handlePongMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::PONG_MESSAGE_ID) {
    int32_t get_index = 0;

    u_int8_t canId = libBufferGet_uint8(msg.buf, &get_index);
    u_int8_t hwType = libBufferGet_uint8(msg.buf, &get_index);

    Serial.println("Pong!");
    Serial.print("CanId: ");
    Serial.print(canId);
    Serial.print(" - HwType: ");
    Serial.println(hwType);
  }
}
void BmsService::handleSetDutyMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::SET_DUTY_MESSAGE_ID) {
    int32_t get_index = 0;
    float duty = (float)libBufferGet_int32(msg.buf, &get_index) / 100000.0f;
    Serial.print("Set ESC Duty: ");
    Serial.println(duty);
  }
}
void BmsService::handleSetCurrentMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::SET_CURRENT_MESSAGE_ID) {
    int32_t get_index = 0;
    float current = (float)libBufferGet_int32(msg.buf, &get_index) / 1000.0f;
    Serial.print("Set ESC Current: ");
    Serial.println(current);
  }
}
void BmsService::handleSetCurrentBrakeMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::SET_CURRENT_BRAKE_MESSAGE_ID) {
    int32_t get_index = 0;
    float brakeCurrent = (float)libBufferGet_int32(msg.buf, &get_index) / 1000.0f;
    Serial.print("Set ESC Brake Current: ");
    Serial.println(brakeCurrent);
  }
}
void BmsService::handleSetRpmMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::SET_RPM_MESSAGE_ID) {
    int32_t get_index = 0;
    int32_t rpm = libBufferGet_int32(msg.buf, &get_index);
    Serial.print("Set ESC RPM: ");
    Serial.println(rpm);
  }
}
void BmsService::handleSetPosMessage(const CAN_message_t &msg) {
  if (msg.id == BmsService::SET_POS_MESSAGE_ID) {
    int32_t get_index = 0;
    float pos = (float)libBufferGet_int32(msg.buf, &get_index) / 1000000.0f;
    Serial.print("Set ESC Position: ");
    Serial.println(pos);
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

    for (uint8_t cellPointer = 0; cellPointer < cellCount; cellPointer++) {
      bool balancing = (bal_state >> cellPointer) & 0b1;
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
}