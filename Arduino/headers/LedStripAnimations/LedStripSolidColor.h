#pragma once

#include "baseAnimation.h"

class LedStripSolidColor : public BaseAnimation
{
    CRGB color;

protected:
    void draw()
    {
        ledStrip->setColor(color);
        ledStrip->Show();
    }

public:
    LedStripSolidColor(LedStrip *ledStrip, CRGB color) : BaseAnimation(ledStrip, 0)
    {
        this->color = color;
    }

    void setColor(CRGB color)
    {
        this->color = color;
        draw();
    }
};
