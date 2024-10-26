#ifndef DcdcService_H
#define DcdcService_H
#include <Arduino.h>

#include "../../observable/observable.h"
#include "../canBus/canBusService.h"

struct DcdcControlData {
  bool controlOrderStart;
  bool controlOrderStop;
  bool protectOrder;
};

struct DcdcComponentStatusData {
  float voltage;
  float current;
  bool overTemperature;
  bool overTemperatureProtection;
  bool inputOverVoltage;
  bool inputUnderVoltage;
  bool outputOverVoltage;
  bool outputUnderVoltage;
  bool outputOverCurrent;
  bool ready;
  bool statusWorking;
  bool hardwareFault;
  bool canCommunicationFault;
  bool fanOn;
  bool shutOffFault;
  bool waterFanOn;
  bool hvilFault;
  int16_t temperature;
};

class DcdcService {
 public:
  static DcdcService &getInstance() {
    static DcdcService instance;
    return instance;
  }

  static const u_int32_t COMPONENT_STATUS_MESSAGE_ID = (u_int32_t)0x1801D08F;
  static const u_int32_t CONTROL_MESSAGE_ID = (u_int32_t)0x18008FD0;

  void printData();

  Observable<DcdcComponentStatusData> componentStatusObservable;
  Observable<DcdcControlData> controlObservable;

 private:
  DcdcService();

  void handleComponentStatusMessage(const CAN_message_t &msg);
  void handleControlMessage(const CAN_message_t &msg);
};

#endif
