#ifndef ControllerService_H
#define ControllerService_H
#include <Arduino.h>

#include "../../observable/observable.h"
#include "../canBus/canBusService.h"
#include "controllerDataTypes.h"

struct ControllerInputStatusData {
  u_int8_t throttleInput;
  int16_t controllerTemperature;
  int16_t motorTemperature;
  bool feedbackStationary;
  bool feedbackForward;
  bool feedbackReverse;
  bool commandNeutral;
  bool commandForward;
  bool commandReverse;
  bool hallA;
  bool hallB;
  bool hallC;
  bool brakeSwitch;
  bool reverseSwitch;
  bool forwardSwitch;
  bool footSwitch;
  bool boostSwitch;
};

struct ControllerComponentStatusData {
  u_int16_t rpm;
  float current;
  float voltage;
  u_int16_t faultState;
  bool canControllerFaultIdentification;
  bool canControllerFaultOverVoltage;
  bool canControllerFaultUnderVoltage;
  bool canControllerFaultStall;
  bool canControllerFaultInternalVolts;
  bool canControllerFaultControllerOverTemperature;
  bool canControllerFaultThrottlePowerUp;
  bool canControllerFaultInternalReset;
  bool canControllerFaultHallThrottleOpenOrShorted;
  bool canControllerFaultAngleSensor;
  bool canControllerFaultMotorOverTemperature;
};

class ControllerService {
 public:
  static ControllerService &getInstance() {
    static ControllerService instance;
    return instance;
  }

  static const u_int32_t COMPONENT_STATUS_MESSAGE_ID = (u_int32_t)0x0CF11E05;
  static const u_int32_t INPUT_STATUS_MESSAGE_ID = (u_int32_t)0x0CF11F05;

  void printData();

  Observable<ControllerInputStatusData> inputStatusObservable;
  Observable<ControllerComponentStatusData> componentStatusObservable;

 private:
  ControllerService();

  void handleInputStatusMessage(const CAN_message_t &msg);
  void handleComponentStatusMessage(const CAN_message_t &msg);
};

#endif
