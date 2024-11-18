
#include "chargingPortInfo.h"
#include <Arduino.h> 
#include <memory>
#include <vector>
#include <string>

ChargingPortInfoPage::~ChargingPortInfoPage() {
  Serial.println("ChargingPortInfoPage destroyed");
  ObcService::getInstance().statusObservable.unsubscribe(pageId);
}
void ChargingPortInfoPage::init(uint64_t id, const Display *d) {
  Page::init(id, d);
  Serial.println("ChargingPortInfoPage created");

  ObcService::getInstance().statusObservable.subscribe(pageId, std::make_unique<std::function<void(ObcStatusData*)>>( [this](ObcStatusData *data) { obcStatusDataChanged(data); }));
  obcStatusData = ObcService::getInstance().statusObservable.getData();

  display->requestRender(0);
}




void ChargingPortInfoPage::obcStatusDataChanged(ObcStatusData *data) {
  obcStatusData = data;
  display->requestRender(0);
}

const char* ChargingPortInfoPage::getStatusString() {

  uint8_t statusCycle = millis() / ChargingPortInfoPage::STATUS_CYCLE_TIME;

  uint8_t statusMessageCount = obcStatusData->socketOverheating +
    obcStatusData->electronicLockPending +
    obcStatusData->electronicLockLocked +
    obcStatusData->electronicLockUnlocked +
    obcStatusData->electronicLockUnlockFault +
    obcStatusData->electronicLockLockedFault +
    obcStatusData->s2SwitchClosed;

  uint8_t messageIndexToShow = (statusCycle % statusMessageCount) + 1;
  uint8_t messagesFound = 0;

  if(obcStatusData->socketOverheating) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ChargingPortInfoPage::STATUS_MESSAGES[socketOverheating];
    }
  }
  

  if(obcStatusData->electronicLockPending) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ChargingPortInfoPage::STATUS_MESSAGES[electronicLockPending];
    }
  }
  

  if(obcStatusData->electronicLockLocked) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ChargingPortInfoPage::STATUS_MESSAGES[electronicLockLocked];
    }
  }
  

  if(obcStatusData->electronicLockUnlocked) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ChargingPortInfoPage::STATUS_MESSAGES[electronicLockUnlocked];
    }
  }
  

  if(obcStatusData->electronicLockUnlockFault) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ChargingPortInfoPage::STATUS_MESSAGES[electronicLockUnlockFault];
    }
  }
  

  if(obcStatusData->electronicLockLockedFault) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ChargingPortInfoPage::STATUS_MESSAGES[electronicLockLockedFault];
    }
  }
  

  if(obcStatusData->s2SwitchClosed) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ChargingPortInfoPage::STATUS_MESSAGES[s2SwitchClosed];
    }
  }
  
  return "UNKNOWN";

}

void ChargingPortInfoPage::render(uint64_t m) {
  Page::render(m);

  display->prepare();

  display->lcd->display(LINES_4);


  display->lcd->locate(0, 7);
  display->lcd->print("         ");

  display->lcd->locate(0, 0);
  display->lcd->print("Charging Port");
  

  display->lcd->locate(1, 0);
  display->lcd->print("CC - ");
  display->lcd->locate(1, 5);

  display->lcd->print(
    obcStatusData->ccSignalNotConnected ? "NO" :
    (obcStatusData->ccSignalHalfConnected ? "Half" :
      (obcStatusData->ccSignalNormalConnected ? "Normal" :
        (obcStatusData->ccSignalResistanceDetectionError ? "Res. Flt." :
          "UNKNOWN"
        )
      )
    )
  );

  display->lcd->locate(2, 0);
  display->lcd->print("CP - ");
  display->lcd->locate(2, 5);
  display->lcd->print(obcStatusData->cpSignalConnected ? "OK" : "NO");


  display->lcd->locate(3, 0);
  display->lcd->print(getStatusString());

 }


