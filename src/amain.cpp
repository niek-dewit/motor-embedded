#include <FlexCAN_T4.h>
#include <map>
#include <./services/bms/bmsDataTypes.h>
#include <./services/controller/controllerDataTypes.h>
#include <libBuffer.h>

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;
std::map<int16_t, int> myMap;
uint8_t bmsId = 0x45;

void canSniff(const CAN_message_t &msg) {
  // From: BMS
  if(false) {
    if(msg.id == ((CAN_PACKET_BMS_TEMPS << 8) | bmsId)) {
      int32_t get_index = 0;

      u_int8_t sensorPointer = libBufferGet_uint8(msg.buf, &get_index);
      u_int8_t temperaturesCount = libBufferGet_uint8(msg.buf, &get_index);

      for (uint8_t i = 0; i < 3; i++) {
        if(sensorPointer + ((get_index - 2) / 2) < temperaturesCount) {
          Serial.print("Sensor: "); Serial.print(sensorPointer + ((get_index - 2) / 2)); Serial.print(" - ");
          float temperature = libBufferGet_float16(msg.buf, 1e2, &get_index);
          Serial.print(temperature); Serial.print("°C ");
        }
      }

      Serial.println();
    }
  }

  // From: BMS
  if(false) {
    if(msg.id == ((CAN_PACKET_BMS_V_TOT << 8) | bmsId)) {
      int32_t get_index = 0;
      float packVoltage = libBufferGet_float32_auto(msg.buf, &get_index);
      float chargerVoltage = libBufferGet_float32_auto(msg.buf, &get_index);
      Serial.print("Vtot. Pack: "); Serial.print(packVoltage); Serial.print("V - "); Serial.print("Vtot. Charger: "); Serial.print(chargerVoltage); Serial.println("V");
    }
    
    if(msg.id == ((CAN_PACKET_BMS_I << 8) | bmsId)) {
      int32_t get_index = 0;
      float packCurrent = libBufferGet_float32_auto(msg.buf, &get_index);
      Serial.print("I: "); Serial.print(packCurrent); Serial.println("A");
    }

    if(msg.id == ((CAN_PACKET_BMS_AH_WH << 8) | bmsId)) {
      int32_t get_index = 0;
      float packAh = libBufferGet_float32_auto(msg.buf, &get_index); // probably not implemented
      float packWh = libBufferGet_float32_auto(msg.buf, &get_index); // probably not implemented
      Serial.print("Capacity: "); Serial.print(packAh); Serial.print("Ah - "); Serial.print(packWh); Serial.println("Wh");
    }
    if(msg.id == ((CAN_PACKET_BMS_HUM << 8) | bmsId)) {
      int32_t get_index = 0;

      float temperatures0 = libBufferGet_float16(msg.buf, 1e2, &get_index); // probably bms circuit board temperature?
      float humidity = libBufferGet_float16(msg.buf, 1e2, &get_index); // no sensor connected
      float temperatures1 = libBufferGet_float16(msg.buf, 1e2, &get_index); // -50, unused?

      Serial.print("Temperature0: "); Serial.print(temperatures0); Serial.print("Temperature1: "); Serial.println(temperatures1);
      Serial.print("Humidity: "); Serial.println(humidity);

    }
  }

  // From: BMS
  if(false) {
    if(msg.id == ((CAN_PACKET_BMS_SOC_SOH_TEMP_STAT << 8) | bmsId)) {
      int32_t get_index = 0;

      float cellVoltageLow = libBufferGet_float16(msg.buf, 1e3, &get_index); //mV
      float cellVoltageHigh = libBufferGet_float16(msg.buf, 1e3, &get_index);  //mV

      u_int8_t currentSoC = libBufferGet_uint8(msg.buf, &get_index);
      u_int8_t maxSoC = libBufferGet_uint8(msg.buf, &get_index);
      u_int8_t tempBatteryHigh = libBufferGet_uint8(msg.buf, &get_index);
      u_int8_t state = libBufferGet_uint8(msg.buf, &get_index);

			bool chargeDesired = state & 0b00000001;
			bool balanceActive = state >> 1 & 0b00000001;
			bool chargeAllowed = state >> 2 & 0b00000001;

      Serial.print("Vlow: "); Serial.print(cellVoltageLow); Serial.print("V - Vhigh: "); Serial.print(cellVoltageHigh); Serial.println("V");
      Serial.print("SoC: "); Serial.print(currentSoC); Serial.print(" / "); Serial.println(maxSoC);
      Serial.print("Temperature high: "); Serial.print(tempBatteryHigh); Serial.println("°C");
      Serial.print("Charge desired: "); Serial.print(chargeDesired); Serial.print(" - Balance active: "); Serial.print(balanceActive); Serial.print(" - Charge allowed: "); Serial.println(chargeAllowed);

    }
  }

  // From: BMS
  if(true) {
    if(msg.id == ((CAN_PACKET_BMS_AH_WH_CHG_TOTAL << 8) | bmsId)) {
      // cannot find firmware implementation, probably empty and not needed
    }
    if(msg.id == ((CAN_PACKET_BMS_AH_WH_DIS_TOTAL << 8) | bmsId)) {
      // cannot find firmware implementation, probably empty and not needed
    }
  }

  // From: BMS
  if(false) {
    if(msg.id == ((CAN_PACKET_BMS_V_CELL << 8) | bmsId)) {
      int32_t get_index = 0;

      u_int8_t cellPointer = libBufferGet_uint8(msg.buf, &get_index);
      u_int8_t cellCount = libBufferGet_uint8(msg.buf, &get_index);

      for (uint8_t i = 0; i < 3; i++) {
        if(cellPointer + ((get_index - 2) / 2) < cellCount) {
          Serial.print("Cell: "); Serial.print(cellPointer + ((get_index - 2) / 2)); Serial.print(" - ");
          float voltage = libBufferGet_float16(msg.buf, 1e3, &get_index);
          Serial.print(voltage); Serial.print("V ");
        }
      }
      Serial.println();

    }
  }

  // From: BMS
  if(false) {
    if(msg.id == ((CAN_PACKET_PONG << 8) | bmsId)) {
      int32_t get_index = 0;

      u_int8_t canId = libBufferGet_uint8(msg.buf, &get_index);
      u_int8_t hwType = libBufferGet_uint8(msg.buf, &get_index);

      Serial.println("Pong!");
      Serial.print("CanId: "); Serial.print(canId); Serial.print(" - HwType: "); Serial.println(hwType);
    }
  }

  // From: BMS
  if(false) {
    if(msg.id == ((CAN_PACKET_SET_DUTY << 8) | bmsId)) {
      int32_t get_index = 0;
      float duty = (float)libBufferGet_int32(msg.buf, &get_index) / 100000.0f;
      Serial.print("Set ESC Duty: "); Serial.println(duty);
    }
    if(msg.id == ((CAN_PACKET_SET_CURRENT << 8) | bmsId)) {
      int32_t get_index = 0;
      float current = (float)libBufferGet_int32(msg.buf, &get_index) / 1000.0f;
      Serial.print("Set ESC Current: "); Serial.println(current);
    }
    if(msg.id == ((CAN_PACKET_SET_CURRENT_BRAKE << 8) | bmsId)) {
      int32_t get_index = 0;
      float brakeCurrent = (float)libBufferGet_int32(msg.buf, &get_index) / 1000.0f;
      Serial.print("Set ESC Brake Current: "); Serial.println(brakeCurrent);
    }
    if(msg.id == ((CAN_PACKET_SET_RPM << 8) | bmsId)) {
      int32_t get_index = 0;
      int32_t rpm = libBufferGet_int32(msg.buf, &get_index);
      Serial.print("Set ESC RPM: "); Serial.println(rpm);
    }
    if(msg.id == ((CAN_PACKET_SET_POS << 8) | bmsId)) {
      int32_t get_index = 0;
      float pos = (float)libBufferGet_int32(msg.buf, &get_index) / 1000000.0f;
      Serial.print("Set ESC Position: "); Serial.println(pos);
    }
  }

  // From: BMS
  if(false) {
    if(msg.id == ((CAN_PACKET_BMS_BAL << 8) | bmsId)) {
      int32_t get_index = 0;

      u_int8_t cellCount = libBufferGet_uint8(msg.buf, &get_index);

      uint64_t bal_state =
        libBufferGet_uint8(msg.buf, &get_index) << 48 |
        libBufferGet_uint8(msg.buf, &get_index) << 40 |
        libBufferGet_uint8(msg.buf, &get_index) << 32 |
        libBufferGet_uint8(msg.buf, &get_index) << 24 |
        libBufferGet_uint8(msg.buf, &get_index) << 16 |
        libBufferGet_uint8(msg.buf, &get_index) << 8 |
        libBufferGet_uint8(msg.buf, &get_index) << 0;

      for (uint8_t cellPointer = 0; cellPointer < cellCount; cellPointer++) {
        bool balancing = (bal_state >> cellPointer) & 0b1;
        Serial.print("Cell: "); Serial.print(cellPointer); Serial.print(" - ");
        Serial.print(balancing); Serial.print(" ");
        if(cellPointer != 0 && (cellPointer + 1) % 3 == 0) {
          Serial.println();
        }
      }
      Serial.println();

    }
  }

  if(false) {
    // From: BMS - To: OBC
    if(msg.id == 0x1806E5F4) {
      int32_t get_index = 0;
      float maxChargingVoltage = (float)libBufferGet_uint16(msg.buf, &get_index) / 10.0f;
      float maxChargingCurrent = (float)libBufferGet_uint16(msg.buf, &get_index) / 10.0f;

      uint8_t controlWorkingState = libBufferGet_uint8(msg.buf, &get_index);
      bool startCharging = controlWorkingState & 0b1;
      bool closeCharger = controlWorkingState >> 1 & 0b1;
      bool sleepCharger = controlWorkingState >> 2 & 0b1;
      
      uint8_t controlOperatingMode = libBufferGet_uint8(msg.buf, &get_index);
      bool chargingMode = controlWorkingState & 0b1;
      bool heatingMode = controlWorkingState >> 1 & 0b1;

      Serial.print("Max Charging Voltage: "); Serial.print(maxChargingVoltage); Serial.print("V ");
      Serial.print("Max Charging Current: "); Serial.print(maxChargingCurrent); Serial.println("A");
      Serial.print("Start Charging: "); Serial.print(startCharging); Serial.print(" ");
      Serial.print("Close Charger: "); Serial.print(closeCharger); Serial.print(" ");
      Serial.print("Sleep Charger: "); Serial.print(sleepCharger); Serial.print(" ");
      Serial.print("Charging Mode: "); Serial.print(chargingMode); Serial.print(" ");
      Serial.print("Heating Mode: "); Serial.println(heatingMode);
    }


    // From: OBC - To: BMS
    if(msg.id == 0x18FF50E5) {
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
      bool electronicLockUnlockFault = electronicLockState == 3; //Control unlock actually detected a lock
      bool electronicLockLockedFault = electronicLockState == 4; //Control lock actually detected unlocked
      bool s2SwitchClosed = chargingPortStatus >> 7 & 0b1;
      int16_t temperature = (int16_t)libBufferGet_uint8(msg.buf, &get_index) - 40;
    
      Serial.print("Max Charging Voltage: "); Serial.print(outputChargingVoltage); Serial.print("V ");
      Serial.print("Max Charging Current: "); Serial.print(outputChargingCurrent); Serial.println("A");
      Serial.print("Temperature: "); Serial.print(temperature); Serial.println("°C ");

      Serial.print("Fault State - hardware protection: "); Serial.print(hardwareProtection); Serial.println(" ");
      Serial.print("Fault State - temperature protection: "); Serial.print(temperatureProtection); Serial.println(" ");
      Serial.print("Fault State - input voltage normal: "); Serial.print(inputVoltageNormal); Serial.println(" ");
      Serial.print("Fault State - input under voltage: "); Serial.print(inputUnderVoltage); Serial.println(" ");
      Serial.print("Fault State - input over voltage: "); Serial.print(inputOverVoltage); Serial.println(" ");
      Serial.print("Fault State - no input voltage: "); Serial.print(noInputVoltage); Serial.println(" ");
      Serial.print("Fault State - output under voltage: "); Serial.print(outputUnderVoltage); Serial.println(" ");
      Serial.print("Fault State - output over voltage: "); Serial.print(outputOverVoltage); Serial.println(" ");
      Serial.print("Fault State - output over current: "); Serial.print(outputOverCurrent); Serial.println(" ");
      Serial.print("Fault State - output short circuit: "); Serial.print(outputShortCircuit); Serial.println(" ");

      Serial.print("Operation Status - communication receive timeout: "); Serial.print(communicationReceiveTimeout); Serial.println(" ");
      Serial.print("Operation Status - working state undefined: "); Serial.print(workingStatusUndefined); Serial.println(" ");
      Serial.print("Operation Status - working state normal: "); Serial.print(workingStatusNormal); Serial.println(" ");
      Serial.print("Operation Status - working state stop: "); Serial.print(workingStatusStop); Serial.println(" ");
      Serial.print("Operation Status - working state stop standby: "); Serial.print(workingStatusStopStandby); Serial.println(" ");
      Serial.print("Operation Status - initialization complete: "); Serial.print(initializationComplete); Serial.println(" ");
      Serial.print("Operation Status - fan on: "); Serial.print(fanOn); Serial.println(" ");
      Serial.print("Operation Status - cooling pump on: "); Serial.print(coolingPumpOn); Serial.println(" ");

      /** Not sure yet if OBC has CC/CP and Electronic lock support */
      Serial.print("Charging Port Status - cc signal not connected: "); Serial.print(ccSignalNotConnected); Serial.println(" ");
      Serial.print("Charging Port Status - cc signal half connected: "); Serial.print(ccSignalHalfConnected); Serial.println(" ");
      Serial.print("Charging Port Status - cc signal normal connected: "); Serial.print(ccSignalNormalConnected); Serial.println(" ");
      Serial.print("Charging Port Status - cc signal resistance detection error: "); Serial.print(ccSignalResistanceDetectionError); Serial.println(" ");
      Serial.print("Charging Port Status - cp signal connected: "); Serial.print(cpSignalConnected); Serial.println(" ");
      Serial.print("Charging Port Status - socket overheating: "); Serial.print(socketOverheating); Serial.println(" ");

      Serial.print("Electronic Lock State - electronic lock pending: "); Serial.print(electronicLockPending); Serial.println(" ");
      Serial.print("Electronic Lock State - electronic lock locked: "); Serial.print(electronicLockLocked); Serial.println(" ");
      Serial.print("Electronic Lock State - electronic lock unlocked: "); Serial.print(electronicLockUnlocked); Serial.println(" ");
      Serial.print("Electronic Lock State - electronic lock unlock fault: "); Serial.print(electronicLockUnlockFault); Serial.println(" ");
      Serial.print("Electronic Lock State - electronic lock locked fault: "); Serial.print(electronicLockLockedFault); Serial.println(" ");
      Serial.print("Electronic Lock State - s2 switch closed: "); Serial.print(s2SwitchClosed); Serial.println(" ");


    }
  }

  // From: Controller
  if(false) {
    if(msg.id == 0x0CF11E05) {
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

      Serial.print("Speed: "); Serial.print(rpm); Serial.print("RPM - ");
      Serial.print("Current: "); Serial.print(current); Serial.print("A - ");
      Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
      Serial.print("Fault State - identification: "); Serial.print(canControllerFaultIdentification); Serial.println(" ");
      Serial.print("Fault State - over voltage: "); Serial.print(canControllerFaultOverVoltage); Serial.println(" ");
      Serial.print("Fault State - under voltage: "); Serial.print(canControllerFaultUnderVoltage); Serial.println(" ");
      Serial.print("Fault State - stall: "); Serial.print(canControllerFaultStall); Serial.println(" ");
      Serial.print("Fault State - internal volts: "); Serial.print(canControllerFaultInternalVolts); Serial.println(" ");
      Serial.print("Fault State - controller over temperature: "); Serial.print(canControllerFaultControllerOverTemperature); Serial.println(" ");
      Serial.print("Fault State - throttle power up: "); Serial.print(canControllerFaultThrottlePowerUp); Serial.println(" ");
      Serial.print("Fault State - internal reset: "); Serial.print(canControllerFaultInternalReset); Serial.println(" ");
      Serial.print("Fault State - hall throttle open or shorted: "); Serial.print(canControllerFaultHallThrottleOpenOrShorted); Serial.println(" ");
      Serial.print("Fault State - angle sensor: "); Serial.print(canControllerFaultAngleSensor); Serial.println(" ");
      Serial.print("Fault State - motor over temperature: "); Serial.print(canControllerFaultMotorOverTemperature); Serial.println(" ");
    }

    if(msg.id == 0x0CF11F05) {
      int32_t get_index = 0;
      u_int8_t throttleInput = libBufferGet_uint8(msg.buf, &get_index);
      int16_t controllerTemperature = (int16_t)libBufferGet_uint8(msg.buf, &get_index) - 40;
      int16_t motorTemperature = (int16_t)libBufferGet_uint8(msg.buf, &get_index) - 30;
      u_int8_t reserved = libBufferGet_uint8(msg.buf, &get_index);
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
      
      Serial.print("Throttle Input: "); Serial.print(throttleInput); Serial.println(" - ");
      Serial.print("Controller Temperature: "); Serial.print(controllerTemperature); Serial.println("°C - ");
      Serial.print("Motor Temperature: "); Serial.print(motorTemperature); Serial.println("°C - ");

      Serial.print("Feedback Stationary: "); Serial.print(feedbackStationary); Serial.println(" ");
      Serial.print("Feedback Forward: "); Serial.print(feedbackForward); Serial.println(" ");
      Serial.print("Feedback Reverse: "); Serial.print(feedbackReverse); Serial.println(" ");
      Serial.print("Command Neutral: "); Serial.print(commandNeutral); Serial.println(" ");
      Serial.print("Command Forward: "); Serial.print(commandForward); Serial.println(" ");
      Serial.print("Command Reverse: "); Serial.print(commandReverse); Serial.println(" ");

      Serial.print("Hall A: "); Serial.print(hallA); Serial.println(" ");
      Serial.print("Hall B: "); Serial.print(hallB); Serial.println(" ");
      Serial.print("Hall C: "); Serial.print(hallC); Serial.println(" ");
      Serial.print("Brake Switch: "); Serial.print(brakeSwitch); Serial.println(" ");
      Serial.print("Reverse Switch: "); Serial.print(reverseSwitch); Serial.println(" ");
      Serial.print("Forward Switch: "); Serial.print(forwardSwitch); Serial.println(" ");
      Serial.print("Foot Switch: "); Serial.print(footSwitch); Serial.println(" ");
      Serial.print("Boost Switch: "); Serial.print(boostSwitch); Serial.println(" ");
    }
  }

  // From: DCDC
  if(false) {
    if(msg.id == 0x1801D08F) {
      int32_t get_index = 0;
      float voltage = (float)libBufferGet_uint16(msg.buf, &get_index) / 10.0;
      float current = (float)libBufferGet_uint16(msg.buf, &get_index) / 10.0;
      u_int16_t status = libBufferGet_uint16(msg.buf, &get_index);
      bool overTemperature = status & 0b1;
      bool overTemperatureProtection = status >> 1 & 0b1;
      bool inputOverVoltage = status >> 2 & 0b1; // Could also be output, docs not clear
      bool inputUnderVoltage = status >> 3 & 0b1; // Could also be output, docs not clear
      bool outputOverVoltage = status >> 4 & 0b1; // Could also be input, docs not clear
      bool outputUnderVoltage = status >> 5 & 0b1; // Could also be input, docs not clear
      bool outputOverCurrent = status >> 6 & 0b1;
      bool ready = status >> 8 & 0b1;
      bool statusWorking = status >> 9 & 0b1;
      bool hardwareFault = status >> 10 & 0b1;
      bool canCommunicationFault = status >> 11 & 0b1;
      bool fanOn = status >> 12 & 0b1;
      bool shutOffFault = status >> 13 & 0b1;
      bool waterFanOn = status >> 14 & 0b1;
      bool hvilFault = status >> 15 & 0b1;
      u_int8_t reserved = libBufferGet_uint8(msg.buf, &get_index);
      int16_t temperature = (int16_t)libBufferGet_uint8(msg.buf, &get_index) - 60;

      Serial.print("DCDC Voltage: "); Serial.print(voltage); Serial.println("V");
      Serial.print("DCDC Current: "); Serial.print(current); Serial.println("A");
      Serial.print("DCDC Temperature: "); Serial.print(temperature); Serial.println("°C");
      Serial.print("DCDC Status - over temperature: "); Serial.print(overTemperature); Serial.println(" ");
      Serial.print("DCDC Status - over temperature protection: "); Serial.print(overTemperatureProtection); Serial.println(" ");
      Serial.print("DCDC Status - input over voltage: "); Serial.print(inputOverVoltage); Serial.println(" ");
      Serial.print("DCDC Status - input under voltage: "); Serial.print(inputUnderVoltage); Serial.println(" ");
      Serial.print("DCDC Status - output over voltage: "); Serial.print(outputOverVoltage); Serial.println(" ");
      Serial.print("DCDC Status - output under voltage: "); Serial.print(outputUnderVoltage); Serial.println(" ");
      Serial.print("DCDC Status - output over current: "); Serial.print(outputOverCurrent); Serial.println(" ");
      Serial.print("DCDC Status - ready: "); Serial.print(ready); Serial.println(" ");
      Serial.print("DCDC Status - status working: "); Serial.print(statusWorking); Serial.println(" ");
      Serial.print("DCDC Status - hardware fault: "); Serial.print(hardwareFault); Serial.println(" ");
      Serial.print("DCDC Status - can communication fault: "); Serial.print(canCommunicationFault); Serial.println(" ");
      Serial.print("DCDC Status - fan on: "); Serial.print(fanOn); Serial.println(" ");
      Serial.print("DCDC Status - shut off fault: "); Serial.print(shutOffFault); Serial.println(" ");
      Serial.print("DCDC Status - water fan on: "); Serial.print(waterFanOn); Serial.println(" ");
      Serial.print("DCDC Status - hvil fault: "); Serial.print(hvilFault); Serial.println(" ");
    
    }
    if(msg.id == 0x18008FD0) { // Can we send these commands from MCU? Docs not clear
      int32_t get_index = 0;
      u_int8_t status = libBufferGet_uint8(msg.buf, &get_index);
      bool controlOrderStart = status & 0b1;
      bool controlOrderStop = status >> 1 & 0b1;
      bool protectOrder = status >> 2 & 0b1;

      Serial.print("Control Order - start: "); Serial.print(controlOrderStart); Serial.println(" ");
      Serial.print("Control Order - stop: "); Serial.print(controlOrderStop); Serial.println(" ");
      Serial.print("Protect Order: "); Serial.print(protectOrder); Serial.println(" ");
    }
  }

  if(false) {
    if (myMap.find(msg.id) == myMap.end()) {
      myMap[msg.id] = 1;
    } else {
      myMap[msg.id]++;
    }
    Serial.println("CAN Monitoring...");
    for (const auto& pair : myMap) {
      Serial.print("ID: ");
      Serial.print(pair.first, HEX);
      Serial.print(" Count: ");
      Serial.println(pair.second);
    }
  }
  
  if(false) {
    Serial.print("MB "); Serial.print(msg.mb);
    Serial.print("  OVERRUN: "); Serial.print(msg.flags.overrun);
    Serial.print("  EXTENDED: "); Serial.print(msg.flags.extended);
    Serial.print("  REMOTE: "); Serial.print(msg.flags.remote);
    Serial.print("  RESERVED: "); Serial.print(msg.flags.reserved);
    Serial.print("  LEN: "); Serial.print(msg.len);
    Serial.print(" EXT: "); Serial.print(msg.flags.extended);
    Serial.print(" TS: "); Serial.print(msg.timestamp);
    Serial.print(" ID: "); Serial.print(msg.id, HEX);
    Serial.print(" BUS: "); Serial.print(msg.bus);
    Serial.print(" Buffer: ");
    for ( uint8_t i = 0; i < msg.len; i++ ) {
      Serial.print(msg.buf[i], HEX); Serial.print(" ");
    } 
    uint64_t number = *((uint64_t*)msg.buf);
    Serial.print(" Decimal: "); Serial.print(number);
    Serial.println();
  }
}
void setup(void) {
  Serial.begin(115200); delay(400);
  Can0.begin();
  Can0.setBaudRate(250000);
  Can0.setMaxMB(16);
  Can0.enableFIFO();
  Can0.enableFIFOInterrupt();
  Can0.onReceive(canSniff);
  Can0.mailboxStatus();
  //    CAN_message_t msg;
  //  msg.id = 0x0;
  //  for ( uint8_t i = 0; i < 8; i++ ) msg.buf[i] = 0b00001111;
  //  Can0.write(msg);
}
static uint32_t timeout = 0;

// 05 22 00 32 00 00 00

void loop() {
  Can0.events();

//  if ( millis() - timeout > 200 ) {
//    Serial.println("..");
//    CAN_message_t msg;
//    msg.id = 0x1;
//    for ( uint8_t i = 0; i < 8; i++ ) msg.buf[i] = i + 1;
//    Can0.write(msg);
//    timeout = millis();
//  }

}