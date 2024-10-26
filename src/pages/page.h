

#ifndef PAGE_H
#define PAGE_H

#include <SSD1803A_I2C.h>
#include "../services/obc/obcService.h"
#include "../services/bms/bmsService.h"
#include <memory>
#include "../devices/display.h"

class Page {
public:
    Page(uint8_t id, Display *display): pageId(id), display(display) {};
    virtual void render(u_int64_t m) {
        lastRendered = m;
    };
    virtual void loop() {};
    
    uint8_t pageId;
    virtual ~Page() {}
    u_int64_t lastRendered = 0;
protected:
    Display *display;

};


class EmptyPage: public Page {  
  
public:
    EmptyPage(Display *display);  
    ~EmptyPage();
    void render(u_int64_t millis) override;
    void loop() override {};
    static constexpr uint8_t ID = 0;
};

class ChargingInfoPage: public Page {
public:
    ChargingInfoPage(Display *display);
    ~ChargingInfoPage();
    void render(u_int64_t millis) override;
    void loop() override {};

    static constexpr uint8_t ID = 1;

private:
    //ObcData *obcData;

    //void obcDataChanged(ObcData *data);
};

class BmsTemperaturesInfoPage: public Page {
public:
    BmsTemperaturesInfoPage(Display *display);
    ~BmsTemperaturesInfoPage();
    void render(u_int64_t millis) override;
    void loop() override {};

    static constexpr uint8_t ID = 1;

private:
    BmsTempsData *bmsTempsData;

    void bmsDataChanged(BmsTempsData *bmsTempsData);
};

class BmsVoltagesInfoPage: public Page {
public:
    BmsVoltagesInfoPage(Display *display, uint8_t pageIndex);
    ~BmsVoltagesInfoPage();
    void render(u_int64_t millis) override;
    void loop() override;

    static constexpr uint8_t ID = 1;
    static constexpr uint8_t PAGE_MAX = 3;
    uint8_t pageIndex = 0;
    uint32_t prevPageSwitch = 0;

private:
    BmsVCellData *bmsVCellData;

    void bmsDataChanged(BmsVCellData *bmsVCellData);
};
#endif 