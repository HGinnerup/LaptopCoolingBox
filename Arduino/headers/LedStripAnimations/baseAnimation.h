#pragma once

#include "../BoxController.h"

class BaseAnimation
{
    uint32_t lastDraw;
    uint16_t drawInterval;
    bool shouldDraw;

protected:
    LedStrip *ledStrip;
    BaseAnimation(LedStrip *ledStrip, uint8_t fps = 60)
    {
        this->ledStrip = ledStrip;
        lastDraw = millis();
        setFps(fps);
    }
    void draw()
    {
        lastDraw = millis();
        setLedColors();
        ledStrip->Show();
    };

public:
    bool getShouldDraw()
    {
        if (!shouldDraw)
            return false;
        return millis() - lastDraw > drawInterval;
    }

    void setFps(uint8_t fps)
    {
        if (fps == 0)
        {
            shouldDraw = false;
            this->drawInterval = 0;
        }
        else
        {
            shouldDraw = true;
            this->drawInterval = 1000 / fps;
        }
    }
    virtual void setLedColors() {}

    void tick()
    {
        if (getShouldDraw())
            draw();
    }
};