#pragma once

#include "BoxController.h"

#define DEBOUNCE_MILLIS 50

class ButtonController
{
    pin pinId;
    bool isToggled;
    bool awaitingDebounce;
    uint32_t debounceMillis;
    bool isCurrentlyClicked;

    void (*onButtonPush)();
    void (*onButtonToggle)(bool state);

    bool isPressed() {
        return digitalRead(pinId) == LOW;
    }

    void _onButtonPush()
    {
        if (onButtonPush != nullptr)
            onButtonPush();

        isToggled = !isToggled;
        if (onButtonToggle != nullptr)
            onButtonToggle(isToggled);
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
                _onButtonPush();
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
    ButtonController(pin pinId, void (*onButtonPush)(), void (*onButtonToggle)(bool state))
    {
        pinMode(pinId, INPUT_PULLUP);
        this->pinId = pinId;
        isToggled = false;
        isCurrentlyClicked = false;

        this->onButtonPush = onButtonPush;
        this->onButtonToggle = onButtonToggle;
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
