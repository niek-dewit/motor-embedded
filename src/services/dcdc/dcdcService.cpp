#include "dcdcService.h"

#include <Arduino.h>
#include <libBuffer.h>

DcdcService::DcdcService() {
  CanBusService::getInstance().registerHandler(DcdcService::COMPONENT_STATUS_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>([this](const CAN_message_t &msg) { handleComponentStatusMessage(msg); }));
  CanBusService::getInstance().registerHandler(DcdcService::CONTROL_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>([this](const CAN_message_t &msg) { handleControlMessage(msg); }));
}

void DcdcService::handleControlMessage(const CAN_message_t &msg) {
  if (msg.id == DcdcService::CONTROL_MESSAGE_ID) {  // Can we send these commands from MCU? Docs not clear
    int32_t get_index = 0;
    u_int8_t status = libBufferGet_uint8(msg.buf, &get_index);
    bool controlOrderStart = status & 0b1;
    bool controlOrderStop = status >> 1 & 0b1;
    bool protectOrder = status >> 2 & 0b1;

    DcdcControlData *dcdcControlData = controlObservable.getData();
    dcdcControlData->controlOrderStart = controlOrderStart;
    dcdcControlData->controlOrderStop = controlOrderStop;
    dcdcControlData->protectOrder = protectOrder;
    controlObservable.notifyListeners();
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
    libBufferGet_uint8(msg.buf, &get_index);  // reserved data
    int16_t temperature = (int16_t)libBufferGet_uint8(msg.buf, &get_index) - 60;

    DcdcComponentStatusData *dcdcComponentStatusData = componentStatusObservable.getData();
    dcdcComponentStatusData->voltage = voltage;
    dcdcComponentStatusData->current = current;
    dcdcComponentStatusData->temperature = temperature;
    dcdcComponentStatusData->overTemperature = overTemperature;
    dcdcComponentStatusData->overTemperatureProtection = overTemperatureProtection;
    dcdcComponentStatusData->inputOverVoltage = inputOverVoltage;
    dcdcComponentStatusData->inputUnderVoltage = inputUnderVoltage;
    dcdcComponentStatusData->outputOverVoltage = outputOverVoltage;
    dcdcComponentStatusData->outputUnderVoltage = outputUnderVoltage;
    dcdcComponentStatusData->outputOverCurrent = outputOverCurrent;
    dcdcComponentStatusData->ready = ready;
    dcdcComponentStatusData->statusWorking = statusWorking;
    dcdcComponentStatusData->hardwareFault = hardwareFault;
    dcdcComponentStatusData->canCommunicationFault = canCommunicationFault;
    dcdcComponentStatusData->fanOn = fanOn;
    dcdcComponentStatusData->shutOffFault = shutOffFault;
    dcdcComponentStatusData->waterFanOn = waterFanOn;
    dcdcComponentStatusData->hvilFault = hvilFault;
    componentStatusObservable.notifyListeners();

  }
}

void DcdcService::printData() {\
  DcdcControlData *dcdcControlData = controlObservable.getData();
  Serial.print("Control Order - start: ");
  Serial.print(dcdcControlData->controlOrderStart);
  Serial.println(" ");
  Serial.print("Control Order - stop: ");
  Serial.print(dcdcControlData->controlOrderStop);
  Serial.println(" ");
  Serial.print("Protect Order: ");
  Serial.print(dcdcControlData->protectOrder);
  Serial.println(" ");

  DcdcComponentStatusData *dcdcComponentStatusData = componentStatusObservable.getData();
  Serial.print("DCDC Voltage: ");
  Serial.print(dcdcComponentStatusData->voltage);
  Serial.println("V");
  Serial.print("DCDC Current: ");
  Serial.print(dcdcComponentStatusData->current);
  Serial.println("A");
  Serial.print("DCDC Temperature: ");
  Serial.print(dcdcComponentStatusData->temperature);
  Serial.println("°C");
  Serial.print("DCDC Status - over temperature: ");
  Serial.print(dcdcComponentStatusData->overTemperature);
  Serial.println(" ");
  Serial.print("DCDC Status - over temperature protection: ");
  Serial.print(dcdcComponentStatusData->overTemperatureProtection);
  Serial.println(" ");
  Serial.print("DCDC Status - input over voltage: ");
  Serial.print(dcdcComponentStatusData->inputOverVoltage);
  Serial.println(" ");
  Serial.print("DCDC Status - input under voltage: ");
  Serial.print(dcdcComponentStatusData->inputUnderVoltage);
  Serial.println(" ");
  Serial.print("DCDC Status - output over voltage: ");
  Serial.print(dcdcComponentStatusData->outputOverVoltage);
  Serial.println(" ");
  Serial.print("DCDC Status - output under voltage: ");
  Serial.print(dcdcComponentStatusData->outputUnderVoltage);
  Serial.println(" ");
  Serial.print("DCDC Status - output over current: ");
  Serial.print(dcdcComponentStatusData->outputOverCurrent);
  Serial.println(" ");
  Serial.print("DCDC Status - ready: ");
  Serial.print(dcdcComponentStatusData->ready);
  Serial.println(" ");
  Serial.print("DCDC Status - status working: ");
  Serial.print(dcdcComponentStatusData->statusWorking);
  Serial.println(" ");
  Serial.print("DCDC Status - hardware fault: ");
  Serial.print(dcdcComponentStatusData->hardwareFault);
  Serial.println(" ");
  Serial.print("DCDC Status - can communication fault: ");
  Serial.print(dcdcComponentStatusData->canCommunicationFault);
  Serial.println(" ");
  Serial.print("DCDC Status - fan on: ");
  Serial.print(dcdcComponentStatusData->fanOn);
  Serial.println(" ");
  Serial.print("DCDC Status - shut off fault: ");
  Serial.print(dcdcComponentStatusData->shutOffFault);
  Serial.println(" ");
  Serial.print("DCDC Status - water fan on: ");
  Serial.print(dcdcComponentStatusData->waterFanOn);
  Serial.println(" ");
  Serial.print("DCDC Status - hvil fault: ");
  Serial.print(dcdcComponentStatusData->hvilFault);
  Serial.println(" ");
}