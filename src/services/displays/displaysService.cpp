#include "displaysService.h"
#include <Arduino.h> 

const uint8_t sdaPin = 18;
const uint8_t sclPin = 19;

DisplaysService::DisplaysService() : displays(), pages(), readyCallback(nullptr) {}

void DisplaysService::init() {
  registerDisplay(DisplayAddress::DISPLAY2, DisplayResetPin::DISPLAY2);
  registerDisplay(DisplayAddress::DISPLAY1, DisplayResetPin::DISPLAY1);

  pinMode(sdaPin, INPUT_PULLUP);
  pinMode(sclPin, INPUT_PULLUP);

  initialized = true;
  if(readyCallback.get() != nullptr) {
    readyCallback->operator()();
  }

}


void DisplaysService::setReadyCallback(std::unique_ptr<std::function<void()>> cb) {
  readyCallback = std::move(cb);
}

void DisplaysService::registerDisplay(DisplayAddress displayId, DisplayResetPin resetPin) {
  pendingRefreshes[displayId] = 0;
  displays[displayId] = std::make_unique<Display>(
    (uint8_t)displayId,
    (uint8_t)resetPin,
    std::make_unique<std::function<void(u_int64_t)>>( [this, displayId](u_int64_t scheduledAfterMillis) {
      refreshRequested(displayId, scheduledAfterMillis);
    })
  );
}

void DisplaysService::refreshRequested(DisplayAddress displayId, u_int64_t scheduledAfterMillis) {
  uint64_t currentMillis = millis();
  pendingRefreshes[displayId] = currentMillis + scheduledAfterMillis;
}

void DisplaysService::checkPendingRefreshes(DisplayAddress displayId) {
  auto page = pages[displayId].get();
  if (page == nullptr) {
    Serial.println("Error: page is null");
    Serial.println((u_int16_t)displayId, HEX);
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
    if(logging) {
      Serial.print("Display rendered: ");
      Serial.println((u_int16_t)displayId, HEX);
    }
    
  }
  

}

void DisplaysService::registerPage(DisplayAddress displayId, std::unique_ptr<Page> page) {
  if(pages.find(displayId) != pages.end()) {
    pages.erase(displayId);
  }

  pages[displayId] = std::move(page);

  pages[displayId]->init(lastPageId++, getDisplay(displayId));

}

const Display* DisplaysService::getDisplay(DisplayAddress displayId) {
  return displays.find(displayId)->second.get();
}

void DisplaysService::reinitializeDisplays() {
  lastDisplayInitializeMillis = millis();
  for (auto& display : displays) {
    display.second->init();
  }
}

void DisplaysService::loop() {

  if(!initialized) {
    if(millis() > 1000) {
      init();
    }
    return;
  }

  if(millis() - lastDisplayInitializeMillis > DISPLAY_REINITIALZE_DELAY) {
    reinitializeDisplays();
  }

  for (auto& page : pages) {
    if(page.second->initialized) {
      page.second->loop();
      checkPendingRefreshes(page.first);
    }
  }

}
