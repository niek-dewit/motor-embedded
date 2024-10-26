#include "interfaceService.h"
#include <Arduino.h> 

const uint8_t sdaPin = 18;
const uint8_t sclPin = 19;

InterfaceService::InterfaceService() : displays(), pages() {
  delay(1000);
  registerDisplay(Display::DISPLAY2, Display::resetPinDisplay2);
  registerDisplay(Display::DISPLAY1, Display::resetPinDisplay1);

  //registerPage(Display::DISPLAY1, std::make_unique<EmptyPage>(displays[Display::DISPLAY1].get()));
  registerPage(Display::DISPLAY1, std::make_unique<BmsVoltagesInfoPage>(displays[Display::DISPLAY1].get(), 0));
  registerPage(Display::DISPLAY2, std::make_unique<BmsTemperaturesInfoPage>(displays[Display::DISPLAY2].get()));

  pinMode(sdaPin, INPUT_PULLUP);
  pinMode(sclPin, INPUT_PULLUP);

  initialized = true;

}

void InterfaceService::registerDisplay(u_int8_t displayId, u_int8_t resetPin) {
  pendingRefreshes[displayId] = 0;
  displays[displayId] = std::make_unique<Display>(
    displayId,
    resetPin,
    std::make_unique<std::function<void(u_int64_t)>>( [this, displayId](u_int64_t scheduledAfterMillis) {
      refreshRequested(displayId, scheduledAfterMillis);
    })
  );
}

void InterfaceService::refreshRequested(u_int8_t displayId, u_int64_t scheduledAfterMillis) {
  uint64_t currentMillis = millis();
  pendingRefreshes[displayId] = currentMillis + scheduledAfterMillis;
}

void InterfaceService::checkPendingRefreshes(u_int8_t displayId) {
  auto page = pages[displayId].get();
  if (page == nullptr) {
    Serial.println("Error: page is null");
    Serial.println(displayId, HEX);
    return;
  }

  uint64_t refreshRequestedOn = pendingRefreshes[displayId];
  uint64_t lastRendered = page->lastRendered;
  uint64_t currentMillis = millis();

  //Serial.print("Refresh check: ");
  //Serial.print(displayId, HEX);
  //Serial.print(" - lastRendered: ");
  //Serial.print(lastRendered);
  //Serial.print(" - refreshRequestedOn: ");
  //Serial.print(refreshRequestedOn);
  //Serial.print(" - currentMillis: ");
  //Serial.println(currentMillis);

  if(refreshRequestedOn != 0 && lastRendered != refreshRequestedOn && refreshRequestedOn < currentMillis && currentMillis - lastRendered > RENDER_DEBOUNCE_TIME) {
    pendingRefreshes[displayId] = 0;

    page->render(currentMillis);
    Serial.print("Display rendered: ");
    Serial.println(displayId, HEX);
  }
  

}

void InterfaceService::registerPage(u_int8_t displayId, std::unique_ptr<Page> page) {
  if(pages.find(displayId) != pages.end()) {
    pages.erase(displayId);
  }

  pages[displayId] = std::move(page);
}

void InterfaceService::loop() {

  if(!initialized) {
    return;
  }

  for (auto& page : pages) {
    page.second->loop();
    checkPendingRefreshes(page.first);
  }

}
