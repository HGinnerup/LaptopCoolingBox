#pragma once

#include "BoxController.h"

template <pin pinId, void (*onButtonPush)(), void (*onButtonToggle)(bool state)>
class ButtonController
{
    static bool isToggled;

    static void _onButtonPush() {
        if(onButtonPush != nullptr)
            onButtonPush();
        
        isToggled = !isToggled;
        if(onButtonToggle != nullptr)
            onButtonToggle(isToggled);
    }

public:
    ButtonController() {
        pinMode(pinId, INPUT_PULLUP);
        attachInterrupt(
            digitalPinToInterrupt(pinId),
            (&_onButtonPush),
            FALLING
        );
    };

    static bool getIsPressed()
    {
        return digitalRead(pinId);
    }

    static bool getIsToggled()
    {
        return isToggled;
    }
};

template <pin pinId, void (*onButtonPush)(), void (*onButtonToggle)(bool state)>
bool ButtonController<pinId, onButtonPush, onButtonToggle>::isToggled = false;
