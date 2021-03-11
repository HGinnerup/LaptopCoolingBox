#pragma once

#include "baseAnimation.h"

class LedStripHueFade : public BaseAnimation
{
    uint8_t hue;

protected:
    void draw()
    {
        hue++;
        CHSV hsv = CHSV(hue, 255, 255);

        ledStrip->setColor(CRGB(hsv));
        ledStrip->Show();
    }

public:
    LedStripHueFade(LedStrip *ledStrip, uint8_t fps, uint8_t hue) : BaseAnimation(ledStrip, fps)
    {
        setHue(hue);
    }

    void setHue(uint8_t hue)
    {
        this->hue = hue;
    }
};
