
#include "obcStateInfo.h"
#include <Arduino.h> 
#include <memory>
#include <vector>
#include <string>


ObcStateInfoPage::~ObcStateInfoPage() {
  Serial.println("ObcStateInfoPage destroyed");
  ObcService::getInstance().statusObservable.unsubscribe(pageId);
}

void ObcStateInfoPage::init(uint64_t id, const Display *d) {
  Page::init(id, d);
  Serial.println("ObcStateInfoPage created");

  ObcService::getInstance().statusObservable.subscribe(pageId, std::make_unique<std::function<void(ObcStatusData*)>>( [this](ObcStatusData *data) { obcStatusDataChanged(data); }));
  obcStatusData = ObcService::getInstance().statusObservable.getData();

  display->requestRender(0);
}



void ObcStateInfoPage::obcStatusDataChanged(ObcStatusData *data) {
  obcStatusData = data;
  display->requestRender(0);
}

const char* ObcStateInfoPage::getStatusString() {

  uint8_t statusCycle = millis() / ObcStateInfoPage::STATUS_CYCLE_TIME;

  uint8_t statusMessageCount = obcStatusData->hardwareProtection +
    obcStatusData->temperatureProtection +
    obcStatusData->inputVoltageNormal +
    obcStatusData->inputUnderVoltage +
    obcStatusData->inputOverVoltage +
    obcStatusData->noInputVoltage +
    obcStatusData->outputUnderVoltage +
    obcStatusData->outputOverVoltage +
    obcStatusData->outputOverCurrent +
    obcStatusData->outputShortCircuit +
    obcStatusData->communicationReceiveTimeout +
    obcStatusData->workingStatusUndefined +
    obcStatusData->workingStatusNormal +
    obcStatusData->workingStatusStop +
    obcStatusData->workingStatusStopStandby +
    obcStatusData->initializationComplete +
    obcStatusData->fanOn +
    obcStatusData->coolingPumpOn;

  uint8_t messageIndexToShow = (statusCycle % statusMessageCount) + 1;
  uint8_t messagesFound = 0;

  if(obcStatusData->hardwareProtection) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ObcStateInfoPage::STATUS_MESSAGES[hardwareProtection];
    }
  }
   
  if(obcStatusData->temperatureProtection) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ObcStateInfoPage::STATUS_MESSAGES[temperatureProtection];
    }
  }
   
  if(obcStatusData->inputVoltageNormal) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ObcStateInfoPage::STATUS_MESSAGES[inputVoltageNormal];
    }
  }
   
  if(obcStatusData->inputUnderVoltage) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ObcStateInfoPage::STATUS_MESSAGES[inputUnderVoltage];
    }
  }
   
  if(obcStatusData->inputOverVoltage) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ObcStateInfoPage::STATUS_MESSAGES[inputOverVoltage];
    }
  }
   
  if(obcStatusData->noInputVoltage) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ObcStateInfoPage::STATUS_MESSAGES[noInputVoltage];
    }
  }
   
  if(obcStatusData->outputUnderVoltage) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ObcStateInfoPage::STATUS_MESSAGES[outputUnderVoltage];
    }
  }
   
  if(obcStatusData->outputOverVoltage) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ObcStateInfoPage::STATUS_MESSAGES[outputOverVoltage];
    }
  }
   
  if(obcStatusData->outputOverCurrent) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ObcStateInfoPage::STATUS_MESSAGES[outputOverCurrent];
    }
  }
   
  if(obcStatusData->outputShortCircuit) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ObcStateInfoPage::STATUS_MESSAGES[outputShortCircuit];
    }
  }
   
  if(obcStatusData->communicationReceiveTimeout) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ObcStateInfoPage::STATUS_MESSAGES[communicationReceiveTimeout];
    }
  }
   
  if(obcStatusData->workingStatusUndefined) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ObcStateInfoPage::STATUS_MESSAGES[workingStatusUndefined];
    }
  }
   
  if(obcStatusData->workingStatusNormal) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ObcStateInfoPage::STATUS_MESSAGES[workingStatusNormal];
    }
  }
   
  if(obcStatusData->workingStatusStop) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ObcStateInfoPage::STATUS_MESSAGES[workingStatusStop];
    }
  }
   
  if(obcStatusData->workingStatusStopStandby) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ObcStateInfoPage::STATUS_MESSAGES[workingStatusStopStandby];
    }
  }
   
  if(obcStatusData->initializationComplete) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ObcStateInfoPage::STATUS_MESSAGES[initializationComplete];
    }
  }
   
  if(obcStatusData->fanOn) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ObcStateInfoPage::STATUS_MESSAGES[fanOn];
    }
  }
   
  if(obcStatusData->coolingPumpOn) {
    messagesFound++;
    if(messagesFound == messageIndexToShow) {
      return ObcStateInfoPage::STATUS_MESSAGES[coolingPumpOn];
    }
  }
  
  return "UNKNOWN";

}

void ObcStateInfoPage::render(uint64_t m) {
  Page::render(m);

  display->prepare();

  display->lcd->display(LINES_4);


  display->lcd->locate(0, 7);
  display->lcd->print("         ");

  display->lcd->locate(0, 0);
  display->lcd->print("OBC - ");
  display->lcd->locate(0, 6);
  display->lcd->print(getStatusString());
  
  display->lcd->locate(1, 0);
  display->lcd->print(obcStatusData->outputChargingVoltage, 1);
  display->lcd->locate(1, 6);
  display->lcd->print("V");

  display->lcd->locate(2, 0);
  display->lcd->print(obcStatusData->outputChargingCurrent, 1);
  display->lcd->locate(2, 6);
  display->lcd->print("A");


  display->lcd->locate(3, 0);
  display->lcd->print(obcStatusData->temperature);
  display->lcd->locate(3, 5);
  display->lcd->write(Display::CELSIUS_ICON);
  display->lcd->locate(3, 6);
  display->lcd->print("C");

 }


