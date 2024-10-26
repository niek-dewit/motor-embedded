#include "canBusService.h"

#include <Arduino.h>
#include <libBuffer.h>

#include "../bms/bmsDataTypes.h"
#include "../bms/bmsService.h"
#include "../controller/controllerDataTypes.h"


void CanBusService::callCanReceived(const CAN_message_t &msg) {
  CanBusService::getInstance().canReceived(msg);
}

CanBusService::CanBusService() {
  Can0.begin();
  Can0.setBaudRate(250000);
  Can0.setMaxMB(16);
  Can0.enableFIFO();
  Can0.enableFIFOInterrupt();
  Can0.onReceive(callCanReceived);
  Can0.mailboxStatus();
}

void CanBusService::registerHandler(uint32_t id, std::unique_ptr<std::function<void(const CAN_message_t &)>> callback) {
  handlers[id] = std::move(callback);
}

void CanBusService::canReceived(const CAN_message_t &msg) {
  if (handlers.count(msg.id)) {
    handlers[msg.id]->operator()(msg);
  } else {
    unknownReceived(msg);
  }
}

void CanBusService::unknownReceived(const CAN_message_t &msg) {
  
  Serial.print("No handler for ID: ");
  Serial.println(msg.id, HEX);
  Serial.print("MB ");
  Serial.print(msg.mb);
  Serial.print("  OVERRUN: ");
  Serial.print(msg.flags.overrun);
  Serial.print("  EXTENDED: ");
  Serial.print(msg.flags.extended);
  Serial.print("  REMOTE: ");
  Serial.print(msg.flags.remote);
  Serial.print("  RESERVED: ");
  Serial.print(msg.flags.reserved);
  Serial.print("  LEN: ");
  Serial.print(msg.len);
  Serial.print(" EXT: ");
  Serial.print(msg.flags.extended);
  Serial.print(" TS: ");
  Serial.print(msg.timestamp);
  Serial.print(" BUS: ");
  Serial.print(msg.bus);
  Serial.print(" Buffer: ");
  for (uint8_t i = 0; i < msg.len; i++) {
    Serial.print(msg.buf[i], HEX);
    Serial.print(" ");
  }
  uint64_t number = *((uint64_t *)msg.buf);
  Serial.print(" Decimal: ");
  Serial.print(number);
  Serial.println();

  if (unknownMessagesMap.find(msg.id) == unknownMessagesMap.end()) {
    unknownMessagesMap[msg.id] = 1;
  } else {
    unknownMessagesMap[msg.id]++;
  }
  Serial.println("Unknown received messages...");
  for (const auto &pair : unknownMessagesMap) {
    Serial.print("ID: ");
    Serial.print(pair.first, HEX);
    Serial.print(" Count: ");
    Serial.println(pair.second);
  }
  
}

void CanBusService::loop() {
  Can0.events();
}
