#pragma once

#define FASTLED_INTERNAL "Stop spamming me!"
#include "FastLED.h"

#include "BoxController.h"

template <pin dataPin, int ledCount>
class LedStripController
{
    CRGB leds[ledCount];

public:
    LedStripController()
    {
        FastLED.addLeds<NEOPIXEL, dataPin>(leds, ledCount);
    }

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
