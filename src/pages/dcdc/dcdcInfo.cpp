
#include "dcdcInfo.h"
#include <Arduino.h> 
#include <memory>
#include <vector>
#include <string>

DcdcInfoPage::DcdcInfoPage(Display *display) :
  Page(DcdcInfoPage::ID, display) {
  Serial.println("DcdcInfoPage created");
  DcdcService::getInstance().componentStatusObservable.subscribe(DcdcInfoPage::ID, std::make_unique<std::function<void(DcdcComponentStatusData*)>>( [this](DcdcComponentStatusData *data) { dcdcComponentStatusDataChanged(data); }));
  dcdcComponentStatusData = DcdcService::getInstance().componentStatusObservable.getData();

  display->requestRender(0);
}

DcdcInfoPage::~DcdcInfoPage() {
  Serial.println("DcdcInfoPage destroyed");
  DcdcService::getInstance().componentStatusObservable.unsubscribe(DcdcInfoPage::ID);
}

void DcdcInfoPage::dcdcComponentStatusDataChanged(DcdcComponentStatusData *data) {
  dcdcComponentStatusData = data;
  display->requestRender(0);
}

const char* DcdcInfoPage::getStatusString() {

  uint8_t statusCycle = millis() / DcdcInfoPage::STATUS_CYCLE_TIME;

  uint8_t statusMessageCount = dcdcComponentStatusData->overTemperature +
    dcdcComponentStatusData->overTemperatureProtection +
    dcdcComponentStatusData->inputOverVoltage +
    dcdcComponentStatusData->inputUnderVoltage +
    dcdcComponentStatusData->outputOverVoltage +
    dcdcComponentStatusData->outputUnderVoltage +
    dcdcComponentStatusData->outputOverCurrent +
    dcdcComponentStatusData->ready +
    dcdcComponentStatusData->statusWorking +
    dcdcComponentStatusData->hardwareFault +
    dcdcComponentStatusData->canCommunicationFault +
    dcdcComponentStatusData->fanOn +
    dcdcComponentStatusData->shutOffFault +
    dcdcComponentStatusData->waterFanOn +
    dcdcComponentStatusData->hvilFault;
  uint8_t messageIndexToShow = (statusCycle % statusMessageCount) + 1;
  uint8_t messagesFound = 0;
  if(dcdcComponentStatusData->overTemperatureProtection) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return DcdcInfoPage::STATUS_MESSAGES[overTemperatureProtection];
    }
  }
  if(dcdcComponentStatusData->inputOverVoltage) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return DcdcInfoPage::STATUS_MESSAGES[inputOverVoltage];
    }
  }
  if(dcdcComponentStatusData->inputUnderVoltage) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return DcdcInfoPage::STATUS_MESSAGES[inputUnderVoltage];
    }
  }
  if(dcdcComponentStatusData->outputOverVoltage) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return DcdcInfoPage::STATUS_MESSAGES[outputOverVoltage];
    }
  }
  if(dcdcComponentStatusData->outputUnderVoltage) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return DcdcInfoPage::STATUS_MESSAGES[outputUnderVoltage];
    }
  }
  if(dcdcComponentStatusData->outputOverCurrent) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return DcdcInfoPage::STATUS_MESSAGES[outputOverCurrent];
    }
  }
  if(dcdcComponentStatusData->ready) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return DcdcInfoPage::STATUS_MESSAGES[ready];
    }
  }
  if(dcdcComponentStatusData->statusWorking) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return DcdcInfoPage::STATUS_MESSAGES[statusWorking];
    }
  }
  if(dcdcComponentStatusData->hardwareFault) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return DcdcInfoPage::STATUS_MESSAGES[hardwareFault];
    }
  }
  if(dcdcComponentStatusData->canCommunicationFault) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return DcdcInfoPage::STATUS_MESSAGES[canCommunicationFault];
    }
  }
  if(dcdcComponentStatusData->fanOn) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return DcdcInfoPage::STATUS_MESSAGES[fanOn];
    }
  }
  if(dcdcComponentStatusData->shutOffFault) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return DcdcInfoPage::STATUS_MESSAGES[shutOffFault];
    }
  }
  if(dcdcComponentStatusData->waterFanOn) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return DcdcInfoPage::STATUS_MESSAGES[waterFanOn];
    }
  }
  if(dcdcComponentStatusData->hvilFault) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return DcdcInfoPage::STATUS_MESSAGES[hvilFault];
    }
  }

  return "UNKNOWN";

}

void DcdcInfoPage::render(uint64_t m) {
  Page::render(m);

  display->prepare();

  display->lcd->display(LINES_4);


  display->lcd->locate(0, 7);
  display->lcd->print("         ");

  display->lcd->locate(0, 0);
  display->lcd->print("DCDC -");
  display->lcd->locate(0, 7);
  display->lcd->print(getStatusString());
  
  display->lcd->locate(1, 0);
  display->lcd->print(dcdcComponentStatusData->voltage, 2);
  display->lcd->locate(1, 5);
  display->lcd->print("V");

  display->lcd->locate(2, 0);
  display->lcd->print(dcdcComponentStatusData->current, 2);
  display->lcd->locate(2, 5);
  display->lcd->print("A");

  display->lcd->locate(3, 0);
  display->lcd->print(dcdcComponentStatusData->temperature);
  display->lcd->locate(3, 4);
  display->lcd->write(Display::CELSIUS_ICON);
  display->lcd->locate(3, 5);
  display->lcd->print("C");
 }


