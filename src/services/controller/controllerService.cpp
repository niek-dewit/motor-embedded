#include <Arduino.h>
#include <libBuffer.h>

#include "controllerService.h"
#include "controllerDataTypes.h"

ControllerService::ControllerService() {
  CanBusService::getInstance().registerHandler(ControllerService::INPUT_STATUS_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>( [this](const CAN_message_t &msg) { handleInputStatusMessage(msg); }));
  CanBusService::getInstance().registerHandler(ControllerService::COMPONENT_STATUS_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>( [this](const CAN_message_t &msg) { handleComponentStatusMessage(msg); }));
}

void ControllerService::handleInputStatusMessage(const CAN_message_t &msg) {
  if (msg.id == ControllerService::INPUT_STATUS_MESSAGE_ID) {
    int32_t get_index = 0;
    u_int8_t throttleInput = libBufferGet_uint8(msg.buf, &get_index);
    int16_t controllerTemperature = (int16_t)libBufferGet_uint8(msg.buf, &get_index) - 40;
    int16_t motorTemperature = (int16_t)libBufferGet_uint8(msg.buf, &get_index) - 30;
    libBufferGet_uint8(msg.buf, &get_index); // Reserved data byte
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

    Serial.print("Throttle Input: ");
    Serial.print(throttleInput);
    Serial.println(" - ");
    Serial.print("Controller Temperature: ");
    Serial.print(controllerTemperature);
    Serial.println("°C - ");
    Serial.print("Motor Temperature: ");
    Serial.print(motorTemperature);
    Serial.println("°C - ");

    Serial.print("Feedback Stationary: ");
    Serial.print(feedbackStationary);
    Serial.println(" ");
    Serial.print("Feedback Forward: ");
    Serial.print(feedbackForward);
    Serial.println(" ");
    Serial.print("Feedback Reverse: ");
    Serial.print(feedbackReverse);
    Serial.println(" ");
    Serial.print("Command Neutral: ");
    Serial.print(commandNeutral);
    Serial.println(" ");
    Serial.print("Command Forward: ");
    Serial.print(commandForward);
    Serial.println(" ");
    Serial.print("Command Reverse: ");
    Serial.print(commandReverse);
    Serial.println(" ");

    Serial.print("Hall A: ");
    Serial.print(hallA);
    Serial.println(" ");
    Serial.print("Hall B: ");
    Serial.print(hallB);
    Serial.println(" ");
    Serial.print("Hall C: ");
    Serial.print(hallC);
    Serial.println(" ");
    Serial.print("Brake Switch: ");
    Serial.print(brakeSwitch);
    Serial.println(" ");
    Serial.print("Reverse Switch: ");
    Serial.print(reverseSwitch);
    Serial.println(" ");
    Serial.print("Forward Switch: ");
    Serial.print(forwardSwitch);
    Serial.println(" ");
    Serial.print("Foot Switch: ");
    Serial.print(footSwitch);
    Serial.println(" ");
    Serial.print("Boost Switch: ");
    Serial.print(boostSwitch);
    Serial.println(" ");
  }
}

void ControllerService::handleComponentStatusMessage(const CAN_message_t &msg) {
  if (msg.id == ControllerService::COMPONENT_STATUS_MESSAGE_ID) {
    int32_t get_index = 0;
    u_int16_t rpm = libBufferGet_uint16(msg.buf, &get_index);
    float current = (float)libBufferGet_uint16(msg.buf, &get_index) / 10.0f;
    float voltage = (float)libBufferGet_uint16(msg.buf, &get_index) / 10.0f;
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

    Serial.print("Speed: ");
    Serial.print(rpm);
    Serial.print("RPM - ");
    Serial.print("Current: ");
    Serial.print(current);
    Serial.print("A - ");
    Serial.print("Voltage: ");
    Serial.print(voltage);
    Serial.println("V");
    Serial.print("Fault State - identification: ");
    Serial.print(canControllerFaultIdentification);
    Serial.println(" ");
    Serial.print("Fault State - over voltage: ");
    Serial.print(canControllerFaultOverVoltage);
    Serial.println(" ");
    Serial.print("Fault State - under voltage: ");
    Serial.print(canControllerFaultUnderVoltage);
    Serial.println(" ");
    Serial.print("Fault State - stall: ");
    Serial.print(canControllerFaultStall);
    Serial.println(" ");
    Serial.print("Fault State - internal volts: ");
    Serial.print(canControllerFaultInternalVolts);
    Serial.println(" ");
    Serial.print("Fault State - controller over temperature: ");
    Serial.print(canControllerFaultControllerOverTemperature);
    Serial.println(" ");
    Serial.print("Fault State - throttle power up: ");
    Serial.print(canControllerFaultThrottlePowerUp);
    Serial.println(" ");
    Serial.print("Fault State - internal reset: ");
    Serial.print(canControllerFaultInternalReset);
    Serial.println(" ");
    Serial.print("Fault State - hall throttle open or shorted: ");
    Serial.print(canControllerFaultHallThrottleOpenOrShorted);
    Serial.println(" ");
    Serial.print("Fault State - angle sensor: ");
    Serial.print(canControllerFaultAngleSensor);
    Serial.println(" ");
    Serial.print("Fault State - motor over temperature: ");
    Serial.print(canControllerFaultMotorOverTemperature);
    Serial.println(" ");
  }
}
