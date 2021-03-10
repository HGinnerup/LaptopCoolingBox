#pragma once

#include "BoxController.h"

#define DEBOUNCE_MILLIS 50

class ButtonController
{
    pin pinId;
    bool awaitingDebounce;
    uint32_t debounceMillis;
    bool isCurrentlyClicked;

    void (*onButtonPush)();

    bool isPressed()
    {
        return digitalRead(pinId) == LOW;
    }

    void debouncedClick()
    {
        if (!awaitingDebounce)
        {
            if (isPressed())
            {
                awaitingDebounce = true;
                debounceMillis = millis();
            }
        }
        else
        {
            if (millis() - debounceMillis < DEBOUNCE_MILLIS)
                return;

            awaitingDebounce = false;

            if (isPressed())
            {
                isCurrentlyClicked = true;
                onButtonPush();
            }
        }
    }

    void debouncedUnClick()
    {
        if (!awaitingDebounce)
        {
            if (!isPressed())
            {
                awaitingDebounce = true;
                debounceMillis = millis();
            }
        }
        else
        {
            if (millis() - debounceMillis < DEBOUNCE_MILLIS)
                return;

            if (!isPressed())
            {
                isCurrentlyClicked = false;
                awaitingDebounce = false;
            }
        }
    }

public:
    ButtonController(pin pinId, void (*onButtonPush)())
    {
        pinMode(pinId, INPUT_PULLUP);
        this->pinId = pinId;
        isCurrentlyClicked = false;

        this->onButtonPush = onButtonPush;
    };

    void Tick()
    {
        if (isCurrentlyClicked)
        {
            debouncedUnClick();
        }
        else
        {
            debouncedClick();
        }
    }
};
