#pragma once

#include "baseAnimation.h"

class LedStripSolidColor : public BaseAnimation
{
    CRGB color;

public:
    LedStripSolidColor(LedStrip *ledStrip, CRGB color) : BaseAnimation(ledStrip, 0)
    {
        this->color = color;
    }

    void setLedColors()
    {
        ledStrip->setColor(color);
    }

    void setColor(CRGB color)
    {
        this->color = color;
        draw();
    }
};
