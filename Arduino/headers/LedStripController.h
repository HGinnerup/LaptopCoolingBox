#pragma once

#define FASTLED_INTERNAL "Stop spamming me!"
#include "FastLED.h"

#include "BoxController.h"

template<pin dataPin, int ledCount>
class LedStripController {
    CRGB leds[ledCount];

    public: 
        LedStripController() {
            FastLED.addLeds<NEOPIXEL, dataPin>(leds, ledCount);
        }
        void setColor(int index, CRGB color) {
            leds[index] = color;
        }
        CRGB getColor(int index) {
            return leds[index];
        }
        void Show() {
            FastLED.show();
        }
};
