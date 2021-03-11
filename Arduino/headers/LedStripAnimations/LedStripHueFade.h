#pragma once

#include "baseAnimation.h"

class LedStripHueFade : public BaseAnimation
{
    uint8_t hue;

public:
    LedStripHueFade(LedStrip *ledStrip, uint8_t fps, uint8_t hue) : BaseAnimation(ledStrip, fps)
    {
        setHue(hue);
    }

    void setLedColors()
    {
        hue++;
        CHSV hsv = CHSV(hue, 255, 255);

        ledStrip->setColor(CRGB(hsv));
    }

    void setHue(uint8_t hue)
    {
        this->hue = hue;
    }
};
