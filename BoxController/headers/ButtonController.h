#pragma once

#include "BoxController.h"

#define DEBOUNCE_MILLIS 5

class ButtonController
{
    pin pinId;
    bool awaitingDebounce;
    uint32_t debounceMillis;
    bool isCurrentlyClicked;

    void (*onButtonPush)();
    void (*onButtonRelease)();

public:
    bool isPressed()
    {
        return digitalRead(pinId) == LOW;
    }

private:
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
                if(onButtonPush != nullptr) 
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
                if(onButtonRelease != nullptr) 
                    onButtonRelease();
            }
        }
    }

public:
    ButtonController(pin pinId, void (*onButtonPush)(), void (*onButtonRelease)() = nullptr)
    {
        pinMode(pinId, INPUT_PULLUP);
        this->pinId = pinId;
        isCurrentlyClicked = false;

        this->onButtonPush = onButtonPush;
        this->onButtonRelease = onButtonRelease;
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
