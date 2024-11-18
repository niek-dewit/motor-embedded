#include "button.h"

Button::Button(uint8_t signalPin): signalPin(signalPin), lastState(false), pressCallback(nullptr) {
                
    bounceButton.attach(signalPin, INPUT_PULLDOWN);
    bounceButton.interval(5);
    bounceButton.setPressedState(HIGH);
}


void Button::update() {
    bounceButton.update();
            
    if (bounceButton.changed()) {
        bool currentState = bounceButton.read();
        if (currentState != lastState) {
            lastState = currentState;
            if(pressCallback.get() != nullptr) {
                pressCallback->operator()(currentState);
            }
        }
    }
}

bool Button::getState() const {
    return bounceButton.read();
}

void Button::setCallback(std::unique_ptr<std::function<void(bool)>> cb) {
    pressCallback = std::move(cb);
}

