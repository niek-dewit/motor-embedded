#include <FlexCAN_T4.h>
#include <map>
#include <./services/bms/bmsDataTypes.h>
#include <libBuffer.h>

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;
std::map<int16_t, int> myMap;
uint8_t bmsId = 0x45;

void canSniff(const CAN_message_t &msg) {
  if(false) {
    if(msg.id == ((CAN_PACKET_BMS_TEMPS << 8) | bmsId)) {
      int32_t get_index = 0;

      uint8_t sensorPointer = msg.buf[get_index++];
      uint8_t temperaturesCount = msg.buf[get_index++];

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

  if(false) {
    if(msg.id == ((CAN_PACKET_BMS_SOC_SOH_TEMP_STAT << 8) | bmsId)) {
      int32_t get_index = 0;

      float cellVoltageLow = libBufferGet_float16(msg.buf, 1e3, &get_index); //mV
      float cellVoltageHigh = libBufferGet_float16(msg.buf, 1e3, &get_index);  //mV

      uint8_t currentSoC = msg.buf[get_index++];
      uint8_t maxSoC = msg.buf[get_index++];
      uint8_t tempBatteryHigh = msg.buf[get_index++];

		  u_int8_t state = msg.buf[get_index++];

			bool chargeDesired = state & 0b00000001;
			bool balanceActive = state >> 1 & 0b00000001;
			bool chargeAllowed = state >> 2 & 0b00000001;

      Serial.print("Vlow: "); Serial.print(cellVoltageLow); Serial.print("V - Vhigh: "); Serial.print(cellVoltageHigh); Serial.println("V");
      Serial.print("SoC: "); Serial.print(currentSoC); Serial.print(" / "); Serial.println(maxSoC);
      Serial.print("Temperature high: "); Serial.print(tempBatteryHigh); Serial.println("°C");
      Serial.print("Charge desired: "); Serial.print(chargeDesired); Serial.print(" - Balance active: "); Serial.print(balanceActive); Serial.print(" - Charge allowed: "); Serial.println(chargeAllowed);

    }
  }

  if(true) {
    if(msg.id == ((CAN_PACKET_BMS_AH_WH_CHG_TOTAL << 8) | bmsId)) {
      // cannot find firmware implementation, probably empty and not needed
    }
    if(msg.id == ((CAN_PACKET_BMS_AH_WH_DIS_TOTAL << 8) | bmsId)) {
      // cannot find firmware implementation, probably empty and not needed
    }
  }

  if(false) {
    if(msg.id == ((CAN_PACKET_BMS_V_CELL << 8) | bmsId)) {
      int32_t get_index = 0;

      uint8_t cellPointer = msg.buf[get_index++];
      uint8_t cellCount = msg.buf[get_index++];

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
  if(false) {
    if(msg.id == ((CAN_PACKET_PONG << 8) | bmsId)) {
      int32_t get_index = 0;

      uint8_t canId = msg.buf[get_index++];
      uint8_t hwType = msg.buf[get_index++];
      Serial.println("Pong!");
      Serial.print("CanId: "); Serial.print(canId); Serial.print(" - HwType: "); Serial.println(hwType);
    }
  }

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


  if(false) {
    if(msg.id == ((CAN_PACKET_BMS_BAL << 8) | bmsId)) {
      int32_t get_index = 0;

      uint8_t cellCount = msg.buf[get_index++];
      uint64_t bal_state =
        msg.buf[get_index++] << 48 |
        msg.buf[get_index++] << 40 |
        msg.buf[get_index++] << 32 |
        msg.buf[get_index++] << 24 |
        msg.buf[get_index++] << 16 |
        msg.buf[get_index++] << 8 |
        msg.buf[get_index++] << 0;

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

  if(true) {
    if(msg.id == 0x1806E5F4) {
      int32_t get_index = 0;
      float maxChargingVoltage = (float)libBufferGet_uint16(msg.buf, &get_index) / 10.0f;
      float maxChargingCurrent = (float)libBufferGet_uint16(msg.buf, &get_index) / 10.0f;
      uint8_t controlWorkingState = msg.buf[get_index++];
      bool startCharging = controlWorkingState & 0b1;
      bool closeCharger = controlWorkingState >> 1 & 0b1;
      bool sleepCharger = controlWorkingState >> 2 & 0b1;
      uint8_t controlOperatingMode = msg.buf[get_index++];
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
    if(msg.id == 0x18FF50E5) {
      int32_t get_index = 0;
      float outputChargingVoltage = (float)libBufferGet_uint16(msg.buf, &get_index) / 10.0f;
      float outputChargingCurrent = (float)libBufferGet_uint16(msg.buf, &get_index) / 10.0f;
      uint8_t faultState = msg.buf[get_index++];
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

      uint8_t operationStatus = msg.buf[get_index++];
      bool communicationReceiveTimeout = operationStatus & 0b1;
      uint8_t workingState = operationStatus >> 1 & 0b11;
      bool workingStatusUndefined = workingState == 0;
      bool workingStatusNormal = workingState == 1;
      bool workingStatusStop = workingState == 2;
      bool workingStatusStopStandby = workingState == 3;
      bool initializationComplete = operationStatus >> 3 & 0b1;
      bool fanOn = operationStatus >> 4 & 0b1;
      bool coolingPumpOn = operationStatus >> 5 & 0b1;

      uint8_t chargingPortStatus = msg.buf[get_index++];
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
      int8_t temperature = msg.buf[get_index++] - 40;
    
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