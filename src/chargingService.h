#ifndef CHARGINGSERVICE_H
#define CHARGINGSERVICE_H

class ChargingService {
public:
    ChargingService(); 

    float getCurrentVoltage();
    float getMaxVoltage();
    float getCurrentAmperage();
    float getMaxAmperage();
    float getMinTemp();
    float getMaxTemp();

    void loop();
private:
    float currentVoltage;
    float maxVoltage;
    float currentAmperage;
    float maxAmperage;
    float minTemp;
    float maxTemp;
    bool updateChargingInfoNeeded = false;
    unsigned long previousMillis = 0;

    void updateChargingInfo();
};

#endif