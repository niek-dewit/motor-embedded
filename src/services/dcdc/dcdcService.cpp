#include "dcdcService.h"

#include <Arduino.h>
#include <libBuffer.h>

DcdcService::DcdcService() {
  CanBusService::getInstance().registerHandler(DcdcService::COMPONENT_STATUS_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>( [this](const CAN_message_t &msg) { handleComponentStatusMessage(msg); }));
  CanBusService::getInstance().registerHandler(DcdcService::CONTROL_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>( [this](const CAN_message_t &msg) { handleControlMessage(msg); }));

}

void DcdcService::handleControlMessage(const CAN_message_t &msg) {
  if (msg.id == DcdcService::CONTROL_MESSAGE_ID) {  // Can we send these commands from MCU? Docs not clear
    int32_t get_index = 0;
    u_int8_t status = libBufferGet_uint8(msg.buf, &get_index);
    bool controlOrderStart = status & 0b1;
    bool controlOrderStop = status >> 1 & 0b1;
    bool protectOrder = status >> 2 & 0b1;

    Serial.print("Control Order - start: ");
    Serial.print(controlOrderStart);
    Serial.println(" ");
    Serial.print("Control Order - stop: ");
    Serial.print(controlOrderStop);
    Serial.println(" ");
    Serial.print("Protect Order: ");
    Serial.print(protectOrder);
    Serial.println(" ");
  }
}

void DcdcService::handleComponentStatusMessage(const CAN_message_t &msg) {
  if (msg.id == DcdcService::COMPONENT_STATUS_MESSAGE_ID) {
    int32_t get_index = 0;
    float voltage = (float)libBufferGet_uint16(msg.buf, &get_index) / 10.0;
    float current = (float)libBufferGet_uint16(msg.buf, &get_index) / 10.0;
    u_int16_t status = libBufferGet_uint16(msg.buf, &get_index);
    bool overTemperature = status & 0b1;
    bool overTemperatureProtection = status >> 1 & 0b1;
    bool inputOverVoltage = status >> 2 & 0b1;    // Could also be output, docs not clear
    bool inputUnderVoltage = status >> 3 & 0b1;   // Could also be output, docs not clear
    bool outputOverVoltage = status >> 4 & 0b1;   // Could also be input, docs not clear
    bool outputUnderVoltage = status >> 5 & 0b1;  // Could also be input, docs not clear
    bool outputOverCurrent = status >> 6 & 0b1;
    bool ready = status >> 8 & 0b1;
    bool statusWorking = status >> 9 & 0b1;
    bool hardwareFault = status >> 10 & 0b1;
    bool canCommunicationFault = status >> 11 & 0b1;
    bool fanOn = status >> 12 & 0b1;
    bool shutOffFault = status >> 13 & 0b1;
    bool waterFanOn = status >> 14 & 0b1;
    bool hvilFault = status >> 15 & 0b1;
    libBufferGet_uint8(msg.buf, &get_index); // reserved data
    int16_t temperature = (int16_t)libBufferGet_uint8(msg.buf, &get_index) - 60;

    Serial.print("DCDC Voltage: ");
    Serial.print(voltage);
    Serial.println("V");
    Serial.print("DCDC Current: ");
    Serial.print(current);
    Serial.println("A");
    Serial.print("DCDC Temperature: ");
    Serial.print(temperature);
    Serial.println("°C");
    Serial.print("DCDC Status - over temperature: ");
    Serial.print(overTemperature);
    Serial.println(" ");
    Serial.print("DCDC Status - over temperature protection: ");
    Serial.print(overTemperatureProtection);
    Serial.println(" ");
    Serial.print("DCDC Status - input over voltage: ");
    Serial.print(inputOverVoltage);
    Serial.println(" ");
    Serial.print("DCDC Status - input under voltage: ");
    Serial.print(inputUnderVoltage);
    Serial.println(" ");
    Serial.print("DCDC Status - output over voltage: ");
    Serial.print(outputOverVoltage);
    Serial.println(" ");
    Serial.print("DCDC Status - output under voltage: ");
    Serial.print(outputUnderVoltage);
    Serial.println(" ");
    Serial.print("DCDC Status - output over current: ");
    Serial.print(outputOverCurrent);
    Serial.println(" ");
    Serial.print("DCDC Status - ready: ");
    Serial.print(ready);
    Serial.println(" ");
    Serial.print("DCDC Status - status working: ");
    Serial.print(statusWorking);
    Serial.println(" ");
    Serial.print("DCDC Status - hardware fault: ");
    Serial.print(hardwareFault);
    Serial.println(" ");
    Serial.print("DCDC Status - can communication fault: ");
    Serial.print(canCommunicationFault);
    Serial.println(" ");
    Serial.print("DCDC Status - fan on: ");
    Serial.print(fanOn);
    Serial.println(" ");
    Serial.print("DCDC Status - shut off fault: ");
    Serial.print(shutOffFault);
    Serial.println(" ");
    Serial.print("DCDC Status - water fan on: ");
    Serial.print(waterFanOn);
    Serial.println(" ");
    Serial.print("DCDC Status - hvil fault: ");
    Serial.print(hvilFault);
    Serial.println(" ");
  }
}
