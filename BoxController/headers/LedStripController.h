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
    LedStripStates preIndicatorState;
    BaseAnimation *currentAnimation;
    uint32_t indicatorStartTime;
    uint16_t indicatorDurationMs;
    uint8_t indicatorPreviousBrightness;
    uint8_t indicatorWidth;
    CRGB *indicatorColor;

public:
    LedStripController(uint16_t indicatorTimeMs = 1000)
    {
        FastLED.addLeds<NEOPIXEL, dataPin>(leds, ledCount);
        ledStrip = LedStrip(leds, ledCount);
        state = LedStripStates::Off;
        indicatorDurationMs = indicatorTimeMs;
    }

    LedStrip *getLedStrip()
    {
        return &ledStrip;
    }

    void setBrightness(uint8_t amount)
    {
        ledStrip.setBrightness(amount);
        ledStrip.Show();
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

    void displayIndicator(CRGB *color, uint8_t width = UINT8_MAX)
    {
        if (state != LedStripStates::Indicator)
        {
            preIndicatorState = state;
            state = LedStripStates::Indicator;
            indicatorPreviousBrightness = getBrightness();
            if (indicatorPreviousBrightness < 32)
                ledStrip.setBrightness(32);
        }
        indicatorStartTime = millis();

        indicatorWidth = width;
        indicatorColor = color;
        drawIndicator();
    }
    void drawIndicator()
    {
        int ledsToLightUp = ledCount * indicatorWidth / UINT8_MAX;

        if (indicatorColor == nullptr && currentAnimation != nullptr)
        {
            currentAnimation->setLedColors();
        }
        else
        {
            for (int i = 0; i < ledsToLightUp; i++)
            {
                ledStrip.setColor(i, *indicatorColor);
            }
        }

        for (int i = ledsToLightUp; i < ledCount; i++)
        {
            ledStrip.setColor(i, CRGB(0, 0, 0));
        }
        ledStrip.Show();
    }
    void indicatorTick()
    {
        if (millis() - indicatorStartTime >= indicatorDurationMs)
        {
            setBrightness(indicatorPreviousBrightness);
            if (state == LedStripStates::Animation)
                setAnimation(currentAnimation);
            else
                state = preIndicatorState;
        }
        else
        {
            if (currentAnimation != nullptr)
            {
                if (currentAnimation->getShouldDraw())
                {
                    drawIndicator();
                }
            }
        }
    }

    void setAnimation(BaseAnimation *animation)
    {
        this->currentAnimation = animation;
        this->state = LedStripStates::Animation;
        currentAnimation->setLedColors();
        ledStrip.Show();
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
            indicatorTick();
            break;

        default:
            break;
        }
    }
};
