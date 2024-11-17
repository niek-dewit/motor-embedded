#include "controllerService.h"

#include <Arduino.h>
#include <libBuffer.h>

#include "controllerDataTypes.h"

ControllerService::ControllerService() {
  CanBusService::getInstance().registerHandler(ControllerService::INPUT_STATUS_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>([this](const CAN_message_t &msg) { handleInputStatusMessage(msg); }));
  CanBusService::getInstance().registerHandler(ControllerService::COMPONENT_STATUS_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>([this](const CAN_message_t &msg) { handleComponentStatusMessage(msg); }));
}

void ControllerService::handleInputStatusMessage(const CAN_message_t &msg) {
  if (msg.id == ControllerService::INPUT_STATUS_MESSAGE_ID) {
    int32_t get_index = 0;
    u_int8_t throttleInput = libBufferGet_uint8(msg.buf, &get_index);
    int16_t controllerTemperature = (int16_t)libBufferGet_uint8(msg.buf, &get_index) - 40;
    int16_t motorTemperature = (int16_t)libBufferGet_uint8(msg.buf, &get_index) - 30;
    libBufferGet_uint8(msg.buf, &get_index);  // Reserved data byte
    u_int8_t controllerStatus = libBufferGet_uint8(msg.buf, &get_index);
    u_int8_t feedbackStatus = (controllerStatus >> 2 & 0b11);
    bool feedbackStationary = feedbackStatus == 0;
    bool feedbackForward = feedbackStatus == 1;
    bool feedbackReverse = feedbackStatus == 2;
    u_int8_t commandStatus = (controllerStatus & 0b11);
    bool commandNeutral = commandStatus == 0;
    bool commandForward = commandStatus == 1;
    bool commandReverse = commandStatus == 2;
    u_int8_t switchStatus = libBufferGet_uint8(msg.buf, &get_index);
    bool hallA = switchStatus & 0b1;
    bool hallB = switchStatus >> 1 & 0b1;
    bool hallC = switchStatus >> 2 & 0b1;
    bool brakeSwitch = switchStatus >> 3 & 0b1;
    bool reverseSwitch = switchStatus >> 4 & 0b1;
    bool forwardSwitch = switchStatus >> 5 & 0b1;
    bool footSwitch = switchStatus >> 6 & 0b1;
    bool boostSwitch = switchStatus >> 7 & 0b1;

    ControllerInputStatusData *controllerInputStatusData = inputStatusObservable.getData();
    controllerInputStatusData->throttleInput = throttleInput;
    controllerInputStatusData->controllerTemperature = controllerTemperature;
    controllerInputStatusData->motorTemperature = motorTemperature;
    controllerInputStatusData->feedbackStationary = feedbackStationary;
    controllerInputStatusData->feedbackForward = feedbackForward;
    controllerInputStatusData->feedbackReverse = feedbackReverse;
    controllerInputStatusData->commandNeutral = commandNeutral;
    controllerInputStatusData->commandForward = commandForward;
    controllerInputStatusData->commandReverse = commandReverse;
    controllerInputStatusData->hallA = hallA;
    controllerInputStatusData->hallB = hallB;
    controllerInputStatusData->hallC = hallC;
    controllerInputStatusData->brakeSwitch = brakeSwitch;
    controllerInputStatusData->reverseSwitch = reverseSwitch;
    controllerInputStatusData->forwardSwitch = forwardSwitch;
    controllerInputStatusData->footSwitch = footSwitch;
    controllerInputStatusData->boostSwitch = boostSwitch;
    inputStatusObservable.notifyListeners();
  }
}

void ControllerService::handleComponentStatusMessage(const CAN_message_t &msg) {
  if (msg.id == ControllerService::COMPONENT_STATUS_MESSAGE_ID) {
    int32_t get_index = 0;
    u_int8_t rpmLSB = libBufferGet_uint8(msg.buf, &get_index);
    u_int8_t rpmMSB = libBufferGet_uint8(msg.buf, &get_index);
    u_int16_t rpm = rpmMSB * 256 + rpmLSB;
    u_int8_t currentLSB = libBufferGet_uint8(msg.buf, &get_index);
    u_int8_t currentMSB = libBufferGet_uint8(msg.buf, &get_index);
    float current = (float)(currentMSB * 256 + currentLSB) / 10.0f;
    u_int8_t voltageLSB = libBufferGet_uint8(msg.buf, &get_index);
    u_int8_t voltageMSB = libBufferGet_uint8(msg.buf, &get_index);
    float voltage = (float)(voltageMSB * 256 + voltageLSB) / 10.0f;

    u_int16_t faultState = libBufferGet_uint16(msg.buf, &get_index);

    bool canControllerFaultIdentification = faultState & 0b1;
    bool canControllerFaultOverVoltage = faultState >> 1 & 0b1;
    bool canControllerFaultUnderVoltage = faultState >> 2 & 0b1;
    bool canControllerFaultStall = faultState >> 4 & 0b1;
    bool canControllerFaultInternalVolts = faultState >> 5 & 0b1;
    bool canControllerFaultControllerOverTemperature = faultState >> 6 & 0b1;
    bool canControllerFaultThrottlePowerUp = faultState >> 7 & 0b1;
    bool canControllerFaultInternalReset = faultState >> 9 & 0b1;
    bool canControllerFaultHallThrottleOpenOrShorted = faultState >> 10 & 0b1;
    bool canControllerFaultAngleSensor = faultState >> 11 & 0b1;
    bool canControllerFaultMotorOverTemperature = faultState >> 14 & 0b1;

    ControllerComponentStatusData *controllerComponentStatusData = componentStatusObservable.getData();
    controllerComponentStatusData->rpm = rpm;
    controllerComponentStatusData->current = current;
    controllerComponentStatusData->voltage = voltage;
    controllerComponentStatusData->faultState = faultState;
    controllerComponentStatusData->canControllerFaultIdentification = canControllerFaultIdentification;
    controllerComponentStatusData->canControllerFaultOverVoltage = canControllerFaultOverVoltage;
    controllerComponentStatusData->canControllerFaultUnderVoltage = canControllerFaultUnderVoltage;
    controllerComponentStatusData->canControllerFaultStall = canControllerFaultStall;
    controllerComponentStatusData->canControllerFaultInternalVolts = canControllerFaultInternalVolts;
    controllerComponentStatusData->canControllerFaultControllerOverTemperature = canControllerFaultControllerOverTemperature;
    controllerComponentStatusData->canControllerFaultThrottlePowerUp = canControllerFaultThrottlePowerUp;
    controllerComponentStatusData->canControllerFaultInternalReset = canControllerFaultInternalReset;
    controllerComponentStatusData->canControllerFaultHallThrottleOpenOrShorted = canControllerFaultHallThrottleOpenOrShorted;
    controllerComponentStatusData->canControllerFaultAngleSensor = canControllerFaultAngleSensor;
    controllerComponentStatusData->canControllerFaultMotorOverTemperature = canControllerFaultMotorOverTemperature;
    componentStatusObservable.notifyListeners();
  }
}
void ControllerService::printData() {
  ControllerInputStatusData *controllerInputStatusData = inputStatusObservable.getData();

  Serial.print("throttleInput: ");
  Serial.print(controllerInputStatusData->throttleInput);
  Serial.println(" ");
  Serial.print("controllerTemperature: ");
  Serial.print(controllerInputStatusData->controllerTemperature);
  Serial.println(" ");
  Serial.print("motorTemperature: ");
  Serial.print(controllerInputStatusData->motorTemperature);
  Serial.println(" ");
  Serial.print("feedbackStationary: ");
  Serial.print(controllerInputStatusData->feedbackStationary);
  Serial.println(" ");
  Serial.print("feedbackForward: ");
  Serial.print(controllerInputStatusData->feedbackForward);
  Serial.println(" ");
  Serial.print("feedbackReverse: ");
  Serial.print(controllerInputStatusData->feedbackReverse);
  Serial.println(" ");
  Serial.print("commandNeutral: ");
  Serial.print(controllerInputStatusData->commandNeutral);
  Serial.println(" ");
  Serial.print("commandForward: ");
  Serial.print(controllerInputStatusData->commandForward);
  Serial.println(" ");
  Serial.print("commandReverse: ");
  Serial.print(controllerInputStatusData->commandReverse);
  Serial.println(" ");
  Serial.print("Hall A: ");
  Serial.print(controllerInputStatusData->hallA);
  Serial.println(" ");
  Serial.print("Hall B: ");
  Serial.print(controllerInputStatusData->hallB);
  Serial.println(" ");
  Serial.print("Hall C: ");
  Serial.print(controllerInputStatusData->hallC);
  Serial.println(" ");
  Serial.print("Brake Switch: ");
  Serial.print(controllerInputStatusData->brakeSwitch);
  Serial.println(" ");
  Serial.print("Reverse Switch: ");
  Serial.print(controllerInputStatusData->reverseSwitch);
  Serial.println(" ");
  Serial.print("Forward Switch: ");
  Serial.print(controllerInputStatusData->forwardSwitch);
  Serial.println(" ");
  Serial.print("Foot Switch: ");
  Serial.print(controllerInputStatusData->footSwitch);
  Serial.println(" ");
  Serial.print("Boost Switch: ");
  Serial.print(controllerInputStatusData->boostSwitch);
  Serial.println(" ");

  ControllerComponentStatusData *controllerComponentStatusData = componentStatusObservable.getData();
  Serial.print("Speed: ");
  Serial.print(controllerComponentStatusData->rpm);
  Serial.print("RPM - ");
  Serial.print("Current: ");
  Serial.print(controllerComponentStatusData->current);
  Serial.print("A - ");
  Serial.print("Voltage: ");
  Serial.print(controllerComponentStatusData->voltage);
  Serial.println("V");
  Serial.print("Fault State - identification: ");
  Serial.print(controllerComponentStatusData->canControllerFaultIdentification);
  Serial.println(" ");
  Serial.print("Fault State - over voltage: ");
  Serial.print(controllerComponentStatusData->canControllerFaultOverVoltage);
  Serial.println(" ");
  Serial.print("Fault State - under voltage: ");
  Serial.print(controllerComponentStatusData->canControllerFaultUnderVoltage);
  Serial.println(" ");
  Serial.print("Fault State - stall: ");
  Serial.print(controllerComponentStatusData->canControllerFaultStall);
  Serial.println(" ");
  Serial.print("Fault State - internal volts: ");
  Serial.print(controllerComponentStatusData->canControllerFaultInternalVolts);
  Serial.println(" ");
  Serial.print("Fault State - controller over temperature: ");
  Serial.print(controllerComponentStatusData->canControllerFaultControllerOverTemperature);
  Serial.println(" ");
  Serial.print("Fault State - throttle power up: ");
  Serial.print(controllerComponentStatusData->canControllerFaultThrottlePowerUp);
  Serial.println(" ");
  Serial.print("Fault State - internal reset: ");
  Serial.print(controllerComponentStatusData->canControllerFaultInternalReset);
  Serial.println(" ");
  Serial.print("Fault State - hall throttle open or shorted: ");
  Serial.print(controllerComponentStatusData->canControllerFaultHallThrottleOpenOrShorted);
  Serial.println(" ");
  Serial.print("Fault State - angle sensor: ");
  Serial.print(controllerComponentStatusData->canControllerFaultAngleSensor);
  Serial.println(" ");
  Serial.print("Fault State - motor over temperature: ");
  Serial.print(controllerComponentStatusData->canControllerFaultMotorOverTemperature);
  Serial.println(" ");
}