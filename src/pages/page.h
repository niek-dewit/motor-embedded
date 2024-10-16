

#ifndef PAGE_H
#define PAGE_H

#include <SSD1803A_I2C.h>
#include "../services/chargingService.h"
#include <memory>

class Page {
public:
    Page(uint8_t id): pageId(id) {};
    virtual void display(SSD1803A_I2C *lcd);
    uint8_t pageId;
    virtual ~Page() {}
};


class EmptyPage: public Page {  
  
public:
    EmptyPage();  
    void display(SSD1803A_I2C *lcd) override;
    static constexpr uint8_t ID = 0;

private:
  char message[16];
};

class ChargingInfoPage: public Page {
public:
    ChargingInfoPage(ChargingService *chargingService);  
    void display(SSD1803A_I2C *lcd) override;
    void setData(float currentVoltage, float maxVoltage, float currentAmperage, float maxAmperage, float minTemp, float maxTemp);

    static constexpr uint8_t ID = 1;

private:
    ChargingService *chargingService;
};


#endif 