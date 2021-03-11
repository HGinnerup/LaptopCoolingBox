#pragma once

#define FASTLED_INTERNAL "Stop spamming me!"
#include "FastLED.h"

#include "BoxController.h"
#include "LedStrip.h"
#include "LedStripAnimations/baseAnimation.h"

enum LedStripStates
{
    Off,
    Indicator,
    Animation
};

template <pin dataPin, int ledCount>
class LedStripController
{
    LedStrip ledStrip;
    CRGB leds[ledCount];
    LedStripStates state;
    BaseAnimation *currentAnimation;
    uint32_t indicatorStartTime;
    uint16_t indicatorDelayMs;

public:
    LedStripController(uint16_t indicatorTimeMs = 1000)
    {
        FastLED.addLeds<NEOPIXEL, dataPin>(leds, ledCount);
        ledStrip = LedStrip(leds, ledCount);
        ledStrip.leds = leds;
        ledStrip.ledCount = ledCount;
        state = LedStripStates::Off;
        indicatorDelayMs = indicatorTimeMs;
    }

    LedStrip *getLedStrip()
    {
        return &ledStrip;
    }

    void setBrightness(uint8_t amount)
    {
        ledStrip.setBrightness(amount);
    }
    uint8_t getBrightness()
    {
        return ledStrip.getBrightness();
    }
    void increaseBrightness(uint8_t amount)
    {
        uint8_t currentPower = getBrightness();
        if (currentPower < 255 - amount)
            setBrightness(currentPower + amount);
        else
            setBrightness(255);
    }
    void decreaseBrightness(uint8_t amount)
    {
        uint8_t currentPower = getBrightness();
        if (currentPower > amount)
            setBrightness(currentPower - amount);
        else
            setBrightness(0);
    }

    void displayIndicator(CRGB color, uint8_t width = UINT8_MAX)
    {
        state = LedStripStates::Indicator;
        indicatorStartTime = millis();

        int ledsToLightUp = ledCount * width / UINT8_MAX;
        for (int i = 0; i < ledsToLightUp; i++)
        {
            setColor(i, color);
        }
        for (int i = ledsToLightUp; i < ledCount; i++)
        {
            setColor(i, CRGB(0, 0, 0));
        }
    }

    void setAnimation(BaseAnimation *animation)
    {
        this->currentAnimation = animation;
        this->state = LedStripStates::Animation;
    }
    BaseAnimation *getAnimation()
    {
        return currentAnimation;
    }

    void turnOff()
    {
        state = LedStripStates::Off;
        ledStrip.setColor(CRGB(0, 0, 0));
        ledStrip.Show();
    }

    void tick()
    {
        switch (state)
        {
        case LedStripStates::Off:
            break;
        case LedStripStates::Animation:
            getAnimation()->tick();
            break;
        case LedStripStates::Indicator:
            break;

        default:
            break;
        }
    }
};
