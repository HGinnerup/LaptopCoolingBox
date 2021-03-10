#pragma once

#include "BoxController.h"

template <pin pinId>
class LedController {
    public:
        LedController() {
            pinMode(pinId, OUTPUT);
        }
        void turnOn() { digitalWrite(pinId, HIGH); }
        void turnOff() { digitalWrite(pinId, LOW); }
};