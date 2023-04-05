#pragma once

#include "BoxController.h"

class LedController {
    pin pinId;
    public:
        LedController(pin pinId) {
            pinMode(pinId, OUTPUT);
            this->pinId = pinId;
        }
        void turnOn() { digitalWrite(pinId, HIGH); }
        void turnOff() { digitalWrite(pinId, LOW); }
};