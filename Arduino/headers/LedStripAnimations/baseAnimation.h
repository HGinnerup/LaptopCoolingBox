#pragma once

#include "../BoxController.h"

class BaseAnimation
{
    uint32_t lastDraw;
    uint16_t drawInterval;
    bool shouldDraw;

protected:
    LedStrip *ledStrip;
    bool drawTimer()
    {
        if (!shouldDraw)
            return false;
        uint32_t ms = millis();
        if (ms - lastDraw > drawInterval)
        {
            lastDraw = ms;
            return true;
        }
        return false;
    }
    BaseAnimation(LedStrip *ledStrip, uint8_t fps = 60)
    {
        this->ledStrip = ledStrip;
        lastDraw = millis();
        setFps(fps);
    }
    virtual void draw() {};

public:
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
    void forceDraw() {
        draw();
    }
    void tick()
    {
        if (drawTimer())
            draw();
    }
};