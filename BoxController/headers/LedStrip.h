#pragma once

#include "BoxController.h"


class LedStrip
{
protected:


public:
    CRGB *leds;
    uint16_t ledCount;
    LedStrip(CRGB leds[], uint16_t ledCount)
    {
        this->leds = leds;
        this->ledCount = ledCount;
    }
    LedStrip() { }

    uint8_t getBrightness()
    {
        return FastLED.getBrightness();
    }

    void setBrightness(uint8_t brightness)
    {
        FastLED.setBrightness(brightness);
    }

    void setColor(CRGB color)
    {
        //FastLED.showColor(color);
        for (int i = 0; i < ledCount; i++)
            setColor(i, color);
    }
    void setColor(int index, CRGB color)
    {
        leds[index] = color;
    }
    CRGB getColor(int index)
    {
        return leds[index];
    }
    void Show()
    {
        FastLED.show();
    }
};