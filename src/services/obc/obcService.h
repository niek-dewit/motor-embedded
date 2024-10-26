#ifndef ObcService_H
#define ObcService_H
#include "../../observable/observable.h"
#include "../canBus/canBusService.h"

struct ObcCommandData {
  float commandMaxChargingVoltage;
  float commandMaxChargingCurrent;
  bool commandStartCharging;
  bool commandCloseCharger;
  bool commandSleepCharger;
  bool commandChargingMode;
  bool commandHeatingMode;
};

struct ObcStatusData {
  float outputChargingVoltage;
  float outputChargingCurrent;
  bool hardwareProtection;
  bool temperatureProtection;
  bool inputVoltageNormal;
  bool inputUnderVoltage;
  bool inputOverVoltage;
  bool noInputVoltage;
  bool outputUnderVoltage;
  bool outputOverVoltage;
  bool outputOverCurrent;
  bool outputShortCircuit;
  bool communicationReceiveTimeout;
  bool workingStatusUndefined;
  bool workingStatusNormal;
  bool workingStatusStop;
  bool workingStatusStopStandby;
  bool initializationComplete;
  bool fanOn;
  bool coolingPumpOn;
  bool ccSignalNotConnected;
  bool ccSignalHalfConnected;
  bool ccSignalNormalConnected;
  bool ccSignalResistanceDetectionError;
  bool cpSignalConnected;
  bool socketOverheating;
  bool electronicLockPending;
  bool electronicLockLocked;
  bool electronicLockUnlocked;
  bool electronicLockUnlockFault;
  bool electronicLockLockedFault;
  bool s2SwitchClosed;
  int16_t temperature;
};

class ObcService {
 public:
  static ObcService &getInstance() {
    static ObcService instance;
    return instance;
  }

  static const u_int32_t CAN_SEND_STATUS_MESSAGE_ID = (u_int32_t)0x18FF50E5;
  static const u_int32_t CAN_RECEIVE_COMMAND_MESSAGE_ID = (u_int32_t)0x1806E5F4;

  void printData();

  Observable<ObcStatusData> statusObservable;
  Observable<ObcCommandData> commandObservable;

 private:
  ObcService();

  void handleStatusMessage(const CAN_message_t &msg);
  void handleCommandMessage(const CAN_message_t &msg);
};

#endif