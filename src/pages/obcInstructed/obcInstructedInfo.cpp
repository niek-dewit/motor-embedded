
#include "obcInstructedInfo.h"
#include <Arduino.h> 
#include <memory>
#include <vector>
#include <string>

ObcInstructedInfoPage::ObcInstructedInfoPage(Display *display) :
  Page(ObcInstructedInfoPage::ID, display) {
  Serial.println("ObcInstructedInfoPage created");
  ObcService::getInstance().commandObservable.subscribe(ObcInstructedInfoPage::ID, std::make_unique<std::function<void(ObcCommandData*)>>( [this](ObcCommandData *data) { obcCommandDataChanged(data); }));
  obcCommandData = ObcService::getInstance().commandObservable.getData();

  display->requestRender(0);
}

ObcInstructedInfoPage::~ObcInstructedInfoPage() {
  Serial.println("ObcInstructedInfoPage destroyed");
  ObcService::getInstance().commandObservable.unsubscribe(ObcInstructedInfoPage::ID);
}

void ObcInstructedInfoPage::obcCommandDataChanged(ObcCommandData *data) {
  obcCommandData = data;
  display->requestRender(0);
}

const char* ObcInstructedInfoPage::getStatusString() {

  uint8_t statusCycle = millis() / ObcInstructedInfoPage::STATUS_CYCLE_TIME;

  uint8_t statusMessageCount = obcCommandData->commandStartCharging +
    obcCommandData->commandCloseCharger +
    obcCommandData->commandSleepCharger +
    obcCommandData->commandChargingMode +
    obcCommandData->commandHeatingMode;
  uint8_t messageIndexToShow = (statusCycle % statusMessageCount) + 1;
  uint8_t messagesFound = 0;
  if(obcCommandData->commandStartCharging) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ObcInstructedInfoPage::STATUS_MESSAGES[commandStartCharging];
    }
  }
  if(obcCommandData->commandCloseCharger) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ObcInstructedInfoPage::STATUS_MESSAGES[commandCloseCharger];
    }
  }
  if(obcCommandData->commandSleepCharger) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ObcInstructedInfoPage::STATUS_MESSAGES[commandSleepCharger];
    }
  }
  if(obcCommandData->commandChargingMode) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ObcInstructedInfoPage::STATUS_MESSAGES[commandChargingMode];
    }
  }
  if(obcCommandData->commandHeatingMode) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ObcInstructedInfoPage::STATUS_MESSAGES[commandHeatingMode];
    }
  }

  return "UNKNOWN";

}

void ObcInstructedInfoPage::render(uint64_t m) {
  Page::render(m);

  display->prepare();

  display->lcd->display(LINES_4);


  display->lcd->locate(0, 7);
  display->lcd->print("         ");

  display->lcd->locate(0, 0);
  display->lcd->print("OBC - Instructed");
  display->lcd->locate(1, 0);
  display->lcd->print(getStatusString());
  
  display->lcd->locate(2, 0);
  display->lcd->print(obcCommandData->commandMaxChargingVoltage, 1);
  display->lcd->locate(2, 6);
  display->lcd->print("Max V.");

  display->lcd->locate(3, 0);
  display->lcd->print(obcCommandData->commandMaxChargingCurrent, 1);
  display->lcd->locate(3, 6);
  display->lcd->print("Max A.");

 }


