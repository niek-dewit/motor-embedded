#include "heartbeatService.h"
#include <Arduino.h> 

HeartbeatService::HeartbeatService() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void HeartbeatService::loop() {
  if (millis() - prevHeartBeat > heartBeatInterval) {
    Serial.print("Heartbeat: "); Serial.println(millis());
    prevHeartBeat = millis();
    blinking = !blinking;
    if (blinking) {
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }
    
  }
}
