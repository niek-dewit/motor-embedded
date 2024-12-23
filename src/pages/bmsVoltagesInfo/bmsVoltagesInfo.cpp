
#include "bmsVoltagesInfo.h"
#include <Arduino.h> 
#include <memory>

BmsVoltagesInfoPage::BmsVoltagesInfoPage(uint8_t pageIndex) :
  pageIndex(pageIndex) {

}

BmsVoltagesInfoPage::~BmsVoltagesInfoPage() {
  Serial.println("BmsVoltagesInfoPage destroyed");
  BmsService::getInstance().bmsTempsObservable.unsubscribe(pageId);
}

void BmsVoltagesInfoPage::init(uint64_t id, const Display *d) {
  Page::init(id, d);
  Serial.println("BmsVoltagesInfoPage created");

  BmsService::getInstance().bmsVCellObservable.subscribe(pageId, std::make_unique<std::function<void(BmsVCellData*)>>( [this](BmsVCellData *data) { bmsDataChanged(data); }));
  bmsVCellData = BmsService::getInstance().bmsVCellObservable.getData();

  display->requestRender(0);
}

void BmsVoltagesInfoPage::bmsDataChanged(BmsVCellData *data) {
  bmsVCellData = data;
  display->requestRender(0);
}

void BmsVoltagesInfoPage::loop() {
  Page::loop();
  if (millis() - prevPageSwitch > 2000) {
    prevPageSwitch = millis();
    pageIndex++;
    if(pageIndex > BmsVoltagesInfoPage::PAGE_MAX) {
      pageIndex = 0;
    }
    display->requestRender(0);
  }
}

void BmsVoltagesInfoPage::render(uint64_t m) {
  Page::render(m);

  display->prepare();
  display->lcd->cls();

  display->lcd->display(LINES_4);

  display->lcd->locate(0, 1);
  display->lcd->print("Groups V.  ");
  display->lcd->print(pageIndex + 1, DEC);
  display->lcd->print("/");
  display->lcd->print(PAGE_MAX + 1, DEC);

  u_int8_t row = 1;
  u_int8_t col = 0;
  u_int8_t maxCol = 2;
  u_int8_t startPointer = pageIndex * 9;
  u_int8_t endPointer = pageIndex * 9 + 9;

  for (const auto &[cellPointer, voltage] : bmsVCellData->cellVoltagesMap) {
    if(cellPointer < startPointer || cellPointer >= endPointer) {
      continue;
    }
    display->lcd->locate(row, 1 + col * 5);
    display->lcd->print(voltage, 2);
    col++;
    if(col > maxCol) {
      col = 0;
      row++;
    }
  }
}


