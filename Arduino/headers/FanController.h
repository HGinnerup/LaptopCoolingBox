#pragma once

#include "BoxController.h"

template <pin speedReadPin, pin speedWritePin>
class FanController
{
    uint8_t targetPower;

public:
    FanController(uint8_t targetPower)
    {
        pinMode(speedReadPin, INPUT_PULLUP);
        pinMode(speedWritePin, OUTPUT);
        this->setPower(targetPower);
    }

    void setPower(uint8_t power)
    {
        this->targetPower = power;
        analogWrite(speedWritePin, power);
    }

    uint32_t getPulseTimeMicros()
    {
        return pulseIn(speedReadPin, HIGH, 1E6); // Timeout at 10 rpm -> quaterRevolutionMicroseconds / 10rpm = 1E6
    }
    uint16_t getRpm(uint32_t halfRevolutionMicros) {
        
        return 15E6 / halfRevolutionMicros; // Microseconds per minute / (quaterRevolutionMicroseconds * 4)
    }
    uint16_t getRpm() {
        return getRpm(getPulseTimeMicros());
    }
    
    uint8_t getPowerTarget()
    {
        return this->targetPower;
    }
};
