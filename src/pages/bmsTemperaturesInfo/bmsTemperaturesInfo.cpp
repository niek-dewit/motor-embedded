
#include "bmsTemperaturesInfo.h"
#include <Arduino.h> 
#include <memory>

BmsTemperaturesInfoPage::~BmsTemperaturesInfoPage() {
  Serial.println("BmsTemperaturesInfoPage destroyed");
  BmsService::getInstance().bmsTempsObservable.unsubscribe(pageId);
}


void BmsTemperaturesInfoPage::init(uint64_t id, const Display *d) {
  Page::init(id, d);
  Serial.println("BmsTemperaturesInfoPage created");

  BmsService::getInstance().bmsTempsObservable.subscribe(pageId, std::make_unique<std::function<void(BmsTempsData*)>>( [this](BmsTempsData *data) { bmsDataChanged(data); }));
  bmsTempsData = BmsService::getInstance().bmsTempsObservable.getData();

  display->requestRender(0);
}

void BmsTemperaturesInfoPage::bmsDataChanged(BmsTempsData *data) {
  bmsTempsData = data;
  display->requestRender(0);
}

void BmsTemperaturesInfoPage::render(uint64_t m) {
  Page::render(m);

  display->prepare();
  display->lcd->cls();

  display->lcd->display(LINES_4);

  display->lcd->locate(0, 1);
  display->lcd->print("BmsT. ");

  u_int8_t row = 0;
  u_int8_t col = 2;
  u_int8_t maxCol = 4;
  int8_t maxTemp = 99;
  int8_t minTemp = -9;
  for (const auto &[sensorPointer, temperature] : bmsTempsData->temperaturesMap) {
    display->lcd->locate(row, 1 + col * 3);
    if(temperature > maxTemp) {
      display->lcd->print("HI");
    } else if (temperature < minTemp) {
      display->lcd->print("LO");
    } else {
      display->lcd->print(temperature, 0);
    }
    col++;
    if(col > maxCol) {
      col = 0;
      row++;
    }
  }


  display->requestRender(2000);

}


