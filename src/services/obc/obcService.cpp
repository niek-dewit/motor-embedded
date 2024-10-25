#include "obcService.h"

#include <Arduino.h>
#include <libBuffer.h>

ObcService::ObcService() {
  currentVoltage = 0.0f;
  maxVoltage = 0.0f;
  currentAmperage = 0.0f;
  maxAmperage = 0.0f;
  minTemp = 0.0f;
  maxTemp = 0.0f;

  CanBusService::getInstance().registerHandler(ObcService::CAN_SEND_STATUS_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>( [this](const CAN_message_t &msg) { handleStatusMessage(msg); }));
  CanBusService::getInstance().registerHandler(ObcService::CAN_RECEIVE_COMMAND_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>( [this](const CAN_message_t &msg) { handleCommandMessage(msg); }));

}
void ObcService::handleCommandMessage(const CAN_message_t &msg) {
  // From: BMS - To: OBC
  if (msg.id == ObcService::CAN_RECEIVE_COMMAND_MESSAGE_ID) {
    int32_t get_index = 0;
    float maxChargingVoltage = (float)libBufferGet_uint16(msg.buf, &get_index) / 10.0f;
    float maxChargingCurrent = (float)libBufferGet_uint16(msg.buf, &get_index) / 10.0f;

    uint8_t controlWorkingState = libBufferGet_uint8(msg.buf, &get_index);
    bool startCharging = controlWorkingState & 0b1;
    bool closeCharger = controlWorkingState >> 1 & 0b1;
    bool sleepCharger = controlWorkingState >> 2 & 0b1;

    uint8_t controlOperatingMode = libBufferGet_uint8(msg.buf, &get_index);
    bool chargingMode = controlOperatingMode & 0b1;
    bool heatingMode = controlOperatingMode >> 1 & 0b1;

    Serial.print("Max Charging Voltage: ");
    Serial.print(maxChargingVoltage);
    Serial.print("V ");
    Serial.print("Max Charging Current: ");
    Serial.print(maxChargingCurrent);
    Serial.println("A");
    Serial.print("Start Charging: ");
    Serial.print(startCharging);
    Serial.print(" ");
    Serial.print("Close Charger: ");
    Serial.print(closeCharger);
    Serial.print(" ");
    Serial.print("Sleep Charger: ");
    Serial.print(sleepCharger);
    Serial.print(" ");
    Serial.print("Charging Mode: ");
    Serial.print(chargingMode);
    Serial.print(" ");
    Serial.print("Heating Mode: ");
    Serial.println(heatingMode);
  }
}

void ObcService::handleStatusMessage(const CAN_message_t &msg) {
  // From: OBC - To: BMS
  if (msg.id == ObcService::CAN_SEND_STATUS_MESSAGE_ID) {
    int32_t get_index = 0;
    float outputChargingVoltage = (float)libBufferGet_uint16(msg.buf, &get_index) / 10.0f;
    float outputChargingCurrent = (float)libBufferGet_uint16(msg.buf, &get_index) / 10.0f;
    uint8_t faultState = libBufferGet_uint8(msg.buf, &get_index);
    bool hardwareProtection = faultState & 0b1;
    bool temperatureProtection = faultState >> 1 & 0b1;
    uint8_t inputVoltageState = faultState >> 2 & 0b11;
    bool inputVoltageNormal = inputVoltageState == 0;
    bool inputUnderVoltage = inputVoltageState == 1;
    bool inputOverVoltage = inputVoltageState == 2;
    bool noInputVoltage = inputVoltageState == 3;
    bool outputUnderVoltage = faultState >> 4 & 0b1;
    bool outputOverVoltage = faultState >> 5 & 0b1;
    bool outputOverCurrent = faultState >> 6 & 0b1;
    bool outputShortCircuit = faultState >> 7 & 0b1;

    uint8_t operationStatus = libBufferGet_uint8(msg.buf, &get_index);
    bool communicationReceiveTimeout = operationStatus & 0b1;
    uint8_t workingState = operationStatus >> 1 & 0b11;
    bool workingStatusUndefined = workingState == 0;
    bool workingStatusNormal = workingState == 1;
    bool workingStatusStop = workingState == 2;
    bool workingStatusStopStandby = workingState == 3;
    bool initializationComplete = operationStatus >> 3 & 0b1;
    bool fanOn = operationStatus >> 4 & 0b1;
    bool coolingPumpOn = operationStatus >> 5 & 0b1;

    uint8_t chargingPortStatus = libBufferGet_uint8(msg.buf, &get_index);
    uint8_t ccSignalState = chargingPortStatus & 0b11;
    bool ccSignalNotConnected = ccSignalState == 0;
    bool ccSignalHalfConnected = ccSignalState == 1;
    bool ccSignalNormalConnected = ccSignalState == 2;
    bool ccSignalResistanceDetectionError = ccSignalState == 3;
    bool cpSignalConnected = chargingPortStatus >> 2 & 0b1;
    bool socketOverheating = chargingPortStatus >> 3 & 0b1;
    uint8_t electronicLockState = chargingPortStatus >> 4 & 0b111;
    bool electronicLockPending = electronicLockState == 0;
    bool electronicLockLocked = electronicLockState == 1;
    bool electronicLockUnlocked = electronicLockState == 2;
    bool electronicLockUnlockFault = electronicLockState == 3;  // Control unlock actually detected a lock
    bool electronicLockLockedFault = electronicLockState == 4;  // Control lock actually detected unlocked
    bool s2SwitchClosed = chargingPortStatus >> 7 & 0b1;
    int16_t temperature = (int16_t)libBufferGet_uint8(msg.buf, &get_index) - 40;

    Serial.print("Max Charging Voltage: ");
    Serial.print(outputChargingVoltage);
    Serial.print("V ");
    Serial.print("Max Charging Current: ");
    Serial.print(outputChargingCurrent);
    Serial.println("A");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println("°C ");

    Serial.print("Fault State - hardware protection: ");
    Serial.print(hardwareProtection);
    Serial.println(" ");
    Serial.print("Fault State - temperature protection: ");
    Serial.print(temperatureProtection);
    Serial.println(" ");
    Serial.print("Fault State - input voltage normal: ");
    Serial.print(inputVoltageNormal);
    Serial.println(" ");
    Serial.print("Fault State - input under voltage: ");
    Serial.print(inputUnderVoltage);
    Serial.println(" ");
    Serial.print("Fault State - input over voltage: ");
    Serial.print(inputOverVoltage);
    Serial.println(" ");
    Serial.print("Fault State - no input voltage: ");
    Serial.print(noInputVoltage);
    Serial.println(" ");
    Serial.print("Fault State - output under voltage: ");
    Serial.print(outputUnderVoltage);
    Serial.println(" ");
    Serial.print("Fault State - output over voltage: ");
    Serial.print(outputOverVoltage);
    Serial.println(" ");
    Serial.print("Fault State - output over current: ");
    Serial.print(outputOverCurrent);
    Serial.println(" ");
    Serial.print("Fault State - output short circuit: ");
    Serial.print(outputShortCircuit);
    Serial.println(" ");

    Serial.print("Operation Status - communication receive timeout: ");
    Serial.print(communicationReceiveTimeout);
    Serial.println(" ");
    Serial.print("Operation Status - working state undefined: ");
    Serial.print(workingStatusUndefined);
    Serial.println(" ");
    Serial.print("Operation Status - working state normal: ");
    Serial.print(workingStatusNormal);
    Serial.println(" ");
    Serial.print("Operation Status - working state stop: ");
    Serial.print(workingStatusStop);
    Serial.println(" ");
    Serial.print("Operation Status - working state stop standby: ");
    Serial.print(workingStatusStopStandby);
    Serial.println(" ");
    Serial.print("Operation Status - initialization complete: ");
    Serial.print(initializationComplete);
    Serial.println(" ");
    Serial.print("Operation Status - fan on: ");
    Serial.print(fanOn);
    Serial.println(" ");
    Serial.print("Operation Status - cooling pump on: ");
    Serial.print(coolingPumpOn);
    Serial.println(" ");

    /** Not sure yet if OBC has CC/CP and Electronic lock support */
    Serial.print("Charging Port Status - cc signal not connected: ");
    Serial.print(ccSignalNotConnected);
    Serial.println(" ");
    Serial.print("Charging Port Status - cc signal half connected: ");
    Serial.print(ccSignalHalfConnected);
    Serial.println(" ");
    Serial.print("Charging Port Status - cc signal normal connected: ");
    Serial.print(ccSignalNormalConnected);
    Serial.println(" ");
    Serial.print("Charging Port Status - cc signal resistance detection error: ");
    Serial.print(ccSignalResistanceDetectionError);
    Serial.println(" ");
    Serial.print("Charging Port Status - cp signal connected: ");
    Serial.print(cpSignalConnected);
    Serial.println(" ");
    Serial.print("Charging Port Status - socket overheating: ");
    Serial.print(socketOverheating);
    Serial.println(" ");

    Serial.print("Electronic Lock State - electronic lock pending: ");
    Serial.print(electronicLockPending);
    Serial.println(" ");
    Serial.print("Electronic Lock State - electronic lock locked: ");
    Serial.print(electronicLockLocked);
    Serial.println(" ");
    Serial.print("Electronic Lock State - electronic lock unlocked: ");
    Serial.print(electronicLockUnlocked);
    Serial.println(" ");
    Serial.print("Electronic Lock State - electronic lock unlock fault: ");
    Serial.print(electronicLockUnlockFault);
    Serial.println(" ");
    Serial.print("Electronic Lock State - electronic lock locked fault: ");
    Serial.print(electronicLockLockedFault);
    Serial.println(" ");
    Serial.print("Electronic Lock State - s2 switch closed: ");
    Serial.print(s2SwitchClosed);
    Serial.println(" ");
  }
}

float ObcService::getCurrentAmperage() {
  return currentAmperage;
}

float ObcService::getCurrentVoltage() {
  return currentVoltage;
}

float ObcService::getMaxAmperage() {
  return maxAmperage;
}

float ObcService::getMaxVoltage() {
  return maxVoltage;
}

float ObcService::getMaxTemp() {
  return maxTemp;
}

float ObcService::getMinTemp() {
  return minTemp;
}
