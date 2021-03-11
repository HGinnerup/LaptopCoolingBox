#pragma once

#include "baseAnimation.h"

class LedStripRpmHue : public BaseAnimation
{
    uint16_t rpm;

public:
    LedStripRpmHue(LedStrip *ledStrip, uint8_t fps, uint16_t rpm) : BaseAnimation(ledStrip, fps)
    {
        setRpm(rpm);
    }

    void setLedColors()
    {
        uint8_t hue = map(rpm, 200, 1900, 0, 255);
        CHSV hsv = CHSV(hue, 255, 255);

        ledStrip->setColor(CRGB(hsv));
    }

    void setRpm(uint16_t rpm)
    {
        this->rpm = rpm;
    }
};
