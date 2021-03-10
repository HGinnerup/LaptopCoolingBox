#pragma once

#include "BoxController.h"

template <pin speedReadPin, pin speedWritePin, void (*onRpmUpdated)(uint16_t rpm)>
class FanController
{
    static uint8_t targetPower;
    //static uint32_t microsAtFalling;
    static uint32_t millisAtRpmUpdate;
    static uint16_t rpm;

    static void interuptFanSenseRising() {
        static uint32_t lastTime = micros();
        uint32_t time = micros();
        uint32_t timeDiff = time - lastTime;
        lastTime = time;
        
        rpm = getRpm(timeDiff);

        millisAtRpmUpdate = millis();

        static bool first = true;

        if(!first && onRpmUpdated != nullptr)
            onRpmUpdated(rpm);
        
        first = false;
    }

public:
    FanController(uint8_t targetPower)
    {
        pinMode(speedReadPin, INPUT_PULLUP);
        pinMode(speedWritePin, OUTPUT);
        this->setPower(targetPower);

        attachInterrupt(
            digitalPinToInterrupt(speedReadPin),
            interuptFanSenseRising,
            FALLING
        );

    }

    void setPower(uint8_t power)
    {
        this->targetPower = power;
        analogWrite(speedWritePin, power);
    }

    static uint32_t getPulseTimeMicros()
    {
        return pulseIn(speedReadPin, HIGH, 1E6); // Timeout at 10 rpm -> quaterRevolutionMicroseconds / 10rpm = 1E6
    }

    static uint16_t getRpm(uint32_t halfRevolutionMicros)
    {
        //return 15E6 / halfRevolutionMicros; // Microseconds per minute / (quaterRevolutionMicroseconds * 4)
        return 30E6 / halfRevolutionMicros; // Microseconds per minute / (quaterRevolutionMicroseconds * 4)
    }

    static uint16_t getRpm()
    {
        //return getRpm(getPulseTimeMicros());
        if(millis() - millisAtRpmUpdate > 15000) // if less than 1 rpm
            return 0;

        return rpm;
    }
    
    static uint8_t getPowerTarget()
    {
        return targetPower;
    }
};

template <pin speedReadPin, pin speedWritePin, void (*onRpmUpdated)(uint16_t rpm)>
uint8_t FanController<speedReadPin, speedWritePin, onRpmUpdated>::targetPower = 0;

template <pin speedReadPin, pin speedWritePin, void (*onRpmUpdated)(uint16_t rpm)>
uint32_t FanController<speedReadPin, speedWritePin, onRpmUpdated>::millisAtRpmUpdate = 0;

template <pin speedReadPin, pin speedWritePin, void (*onRpmUpdated)(uint16_t rpm)>
uint16_t FanController<speedReadPin, speedWritePin, onRpmUpdated>::rpm = 0;
