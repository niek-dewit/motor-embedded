#include "obcService.h"

#include <Arduino.h>
#include <libBuffer.h>

ObcService::ObcService() {
  CanBusService::getInstance().registerHandler(ObcService::CAN_SEND_STATUS_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>([this](const CAN_message_t &msg) { handleStatusMessage(msg); }));
  CanBusService::getInstance().registerHandler(ObcService::CAN_RECEIVE_COMMAND_MESSAGE_ID, std::make_unique<std::function<void(const CAN_message_t &)>>([this](const CAN_message_t &msg) { handleCommandMessage(msg); }));
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

    ObcCommandData *obcCommandData = commandObservable.getData();
    obcCommandData->commandMaxChargingVoltage = maxChargingVoltage;
    obcCommandData->commandMaxChargingCurrent = maxChargingCurrent;
    obcCommandData->commandStartCharging = startCharging;
    obcCommandData->commandCloseCharger = closeCharger;
    obcCommandData->commandSleepCharger = sleepCharger;
    obcCommandData->commandChargingMode = chargingMode;
    obcCommandData->commandHeatingMode = heatingMode;
    commandObservable.notifyListeners();

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

    ObcStatusData *obcStatusData = statusObservable.getData();
    obcStatusData->outputChargingVoltage = outputChargingVoltage;
    obcStatusData->outputChargingCurrent = outputChargingCurrent;
    obcStatusData->hardwareProtection = hardwareProtection;
    obcStatusData->temperatureProtection = temperatureProtection;
    obcStatusData->inputVoltageNormal = inputVoltageNormal;
    obcStatusData->inputUnderVoltage = inputUnderVoltage;
    obcStatusData->inputOverVoltage = inputOverVoltage;
    obcStatusData->noInputVoltage = noInputVoltage;
    obcStatusData->outputUnderVoltage = outputUnderVoltage;
    obcStatusData->outputOverVoltage = outputOverVoltage;
    obcStatusData->outputOverCurrent = outputOverCurrent;
    obcStatusData->outputShortCircuit = outputShortCircuit;
    obcStatusData->communicationReceiveTimeout = communicationReceiveTimeout;
    obcStatusData->workingStatusUndefined = workingStatusUndefined;
    obcStatusData->workingStatusNormal = workingStatusNormal;
    obcStatusData->workingStatusStop = workingStatusStop;
    obcStatusData->workingStatusStopStandby = workingStatusStopStandby;
    obcStatusData->initializationComplete = initializationComplete;
    obcStatusData->fanOn = fanOn;
    obcStatusData->coolingPumpOn = coolingPumpOn;
    obcStatusData->ccSignalNotConnected = ccSignalNotConnected;
    obcStatusData->ccSignalHalfConnected = ccSignalHalfConnected;
    obcStatusData->ccSignalNormalConnected = ccSignalNormalConnected;
    obcStatusData->ccSignalResistanceDetectionError = ccSignalResistanceDetectionError;
    obcStatusData->cpSignalConnected = cpSignalConnected;
    obcStatusData->socketOverheating = socketOverheating;
    obcStatusData->electronicLockPending = electronicLockPending;
    obcStatusData->electronicLockLocked = electronicLockLocked;
    obcStatusData->electronicLockUnlocked = electronicLockUnlocked;
    obcStatusData->electronicLockUnlockFault = electronicLockUnlockFault;
    obcStatusData->electronicLockLockedFault = electronicLockLockedFault;
    obcStatusData->s2SwitchClosed = s2SwitchClosed;
    obcStatusData->temperature = temperature;
    statusObservable.notifyListeners();

  }
}

void ObcService::printData() {
  ObcCommandData *obcCommandData = commandObservable.getData();
  Serial.print("Max Charging Voltage: ");
  Serial.print(obcCommandData->commandMaxChargingVoltage);
  Serial.print("V ");
  Serial.print("Max Charging Current: ");
  Serial.print(obcCommandData->commandMaxChargingCurrent);
  Serial.println("A");
  Serial.print("Start Charging: ");
  Serial.print(obcCommandData->commandStartCharging);
  Serial.print(" ");
  Serial.print("Close Charger: ");
  Serial.print(obcCommandData->commandCloseCharger);
  Serial.print(" ");
  Serial.print("Sleep Charger: ");
  Serial.print(obcCommandData->commandSleepCharger);
  Serial.print(" ");
  Serial.print("Charging Mode: ");
  Serial.print(obcCommandData->commandChargingMode);
  Serial.print(" ");
  Serial.print("Heating Mode: ");
  Serial.println(obcCommandData->commandHeatingMode);

  ObcStatusData *obcStatusData = statusObservable.getData();
  Serial.print("Max Charging Voltage: ");
  Serial.print(obcStatusData->outputChargingVoltage);
  Serial.print("V ");
  Serial.print("Max Charging Current: ");
  Serial.print(obcStatusData->outputChargingCurrent);
  Serial.println("A");
  Serial.print("Temperature: ");
  Serial.print(obcStatusData->temperature);
  Serial.println("°C ");

  Serial.print("Fault State - hardware protection: ");
  Serial.print(obcStatusData->hardwareProtection);
  Serial.println(" ");
  Serial.print("Fault State - temperature protection: ");
  Serial.print(obcStatusData->temperatureProtection);
  Serial.println(" ");
  Serial.print("Fault State - input voltage normal: ");
  Serial.print(obcStatusData->inputVoltageNormal);
  Serial.println(" ");
  Serial.print("Fault State - input under voltage: ");
  Serial.print(obcStatusData->inputUnderVoltage);
  Serial.println(" ");
  Serial.print("Fault State - input over voltage: ");
  Serial.print(obcStatusData->inputOverVoltage);
  Serial.println(" ");
  Serial.print("Fault State - no input voltage: ");
  Serial.print(obcStatusData->noInputVoltage);
  Serial.println(" ");
  Serial.print("Fault State - output under voltage: ");
  Serial.print(obcStatusData->outputUnderVoltage);
  Serial.println(" ");
  Serial.print("Fault State - output over voltage: ");
  Serial.print(obcStatusData->outputOverVoltage);
  Serial.println(" ");
  Serial.print("Fault State - output over current: ");
  Serial.print(obcStatusData->outputOverCurrent);
  Serial.println(" ");
  Serial.print("Fault State - output short circuit: ");
  Serial.print(obcStatusData->outputShortCircuit);
  Serial.println(" ");

  Serial.print("Operation Status - communication receive timeout: ");
  Serial.print(obcStatusData->communicationReceiveTimeout);
  Serial.println(" ");
  Serial.print("Operation Status - working state undefined: ");
  Serial.print(obcStatusData->workingStatusUndefined);
  Serial.println(" ");
  Serial.print("Operation Status - working state normal: ");
  Serial.print(obcStatusData->workingStatusNormal);
  Serial.println(" ");
  Serial.print("Operation Status - working state stop: ");
  Serial.print(obcStatusData->workingStatusStop);
  Serial.println(" ");
  Serial.print("Operation Status - working state stop standby: ");
  Serial.print(obcStatusData->workingStatusStopStandby);
  Serial.println(" ");
  Serial.print("Operation Status - initialization complete: ");
  Serial.print(obcStatusData->initializationComplete);
  Serial.println(" ");
  Serial.print("Operation Status - fan on: ");
  Serial.print(obcStatusData->fanOn);
  Serial.println(" ");
  Serial.print("Operation Status - cooling pump on: ");
  Serial.print(obcStatusData->coolingPumpOn);
  Serial.println(" ");

  /** Not sure yet if OBC has CC/CP and Electronic lock support */
  Serial.print("Charging Port Status - cc signal not connected: ");
  Serial.print(obcStatusData->ccSignalNotConnected);
  Serial.println(" ");
  Serial.print("Charging Port Status - cc signal half connected: ");
  Serial.print(obcStatusData->ccSignalHalfConnected);
  Serial.println(" ");
  Serial.print("Charging Port Status - cc signal normal connected: ");
  Serial.print(obcStatusData->ccSignalNormalConnected);
  Serial.println(" ");
  Serial.print("Charging Port Status - cc signal resistance detection error: ");
  Serial.print(obcStatusData->ccSignalResistanceDetectionError);
  Serial.println(" ");
  Serial.print("Charging Port Status - cp signal connected: ");
  Serial.print(obcStatusData->cpSignalConnected);
  Serial.println(" ");
  Serial.print("Charging Port Status - socket overheating: ");
  Serial.print(obcStatusData->socketOverheating);
  Serial.println(" ");

  Serial.print("Electronic Lock State - electronic lock pending: ");
  Serial.print(obcStatusData->electronicLockPending);
  Serial.println(" ");
  Serial.print("Electronic Lock State - electronic lock locked: ");
  Serial.print(obcStatusData->electronicLockLocked);
  Serial.println(" ");
  Serial.print("Electronic Lock State - electronic lock unlocked: ");
  Serial.print(obcStatusData->electronicLockUnlocked);
  Serial.println(" ");
  Serial.print("Electronic Lock State - electronic lock unlock fault: ");
  Serial.print(obcStatusData->electronicLockUnlockFault);
  Serial.println(" ");
  Serial.print("Electronic Lock State - electronic lock locked fault: ");
  Serial.print(obcStatusData->electronicLockLockedFault);
  Serial.println(" ");
  Serial.print("Electronic Lock State - s2 switch closed: ");
  Serial.print(obcStatusData->s2SwitchClosed);
  Serial.println(" ");
}