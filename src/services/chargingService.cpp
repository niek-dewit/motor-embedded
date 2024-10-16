#include "chargingService.h"
#include <Arduino.h> 

ChargingService::ChargingService() {
    currentVoltage = 0.0f;
    maxVoltage = 0.0f;
    currentAmperage = 0.0f;
    maxAmperage = 0.0f;
    minTemp = 0.0f;
    maxTemp = 0.0f;
}

float ChargingService::getCurrentAmperage() {
    return currentAmperage;
}

float ChargingService::getCurrentVoltage() {
    return currentVoltage;
}

float ChargingService::getMaxAmperage() {
    return maxAmperage;
}

float ChargingService::getMaxVoltage() {
    return maxVoltage;
}

float ChargingService::getMaxTemp() {
    return maxTemp;
}

float ChargingService::getMinTemp() {
    return minTemp;
}

void ChargingService::loop() {
  unsigned long currentMillis = millis();
  bool checkUpdateChargingInfo = currentMillis /1000 % 2;
  if(updateChargingInfoNeeded != checkUpdateChargingInfo) {
    updateChargingInfoNeeded = checkUpdateChargingInfo;  
    updateChargingInfo();
  };
}

void ChargingService::updateChargingInfo() {
  currentVoltage = ((float)random(1200, 1230))/10.0f;
  maxVoltage = ((float)random(1250, 1350))/10.0f;
  currentAmperage = ((float)random(80, 120))/10.0f;
  maxAmperage = ((float)random(140, 160))/10.0f;
  minTemp = ((float)random(150, 200))/10.0f;
  maxTemp = ((float)random(220, 250))/10.0f;
}
