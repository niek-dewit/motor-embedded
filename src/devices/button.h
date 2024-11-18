#ifndef BUTTON_H
#define BUTTON_H

#include <memory>
#include <functional>
#include <Bounce2.h>

class Button {
public:

    void setCallback(std::unique_ptr<std::function<void(bool)>> pressCallback);

    void update();
    bool getState() const;
    Button(uint8_t signalPin); 


private:

    Bounce2::Button bounceButton;
    uint8_t signalPin;
    bool lastState;
    std::unique_ptr<std::function<void(bool)>> pressCallback;
};

#endif 