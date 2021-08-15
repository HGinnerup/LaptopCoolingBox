#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT_BUTTON
//#define DEBUG_PRINT_RPM
#define DEBUG_PRINT_TABLEHEIGHT
#endif

#include "headers/BoxController.h"

#define FAN_READ_PIN 2
#define FAN_WRITE_PIN 3

#define LED_STRIP_PIN 5
#define LED_STRIP_LENGTH 23
#define LED_BUTTON_MODE_PIN 7
#define LED_BUTTON_INCREMENT_PIN 6
#define LED_BUTTON_DECREMENT_PIN 8

#define FAN_BUTTON_MODE_PIN 4
#define FAN_BUTTON_INCREMENT_PIN 9
#define FAN_BUTTON_DECREMENT_PIN 11

#define TABLE_HEIGHT_RAISE_PIN 12
#define TABLE_HEIGHT_LOWER_PIN 13

void ledStripOnRpmUpdate(uint16_t rpm);
auto fanController = FanController<FAN_READ_PIN, FAN_WRITE_PIN, ledStripOnRpmUpdate>(0);

void fanModeIterate();
auto fanButtonMode = ButtonController(FAN_BUTTON_MODE_PIN, fanModeIterate);
void fanIncrement();
auto fanButtonIncrement = ButtonController(FAN_BUTTON_INCREMENT_PIN, fanIncrement);
void fanDecrement();
auto fanButtonDecrement = ButtonController(FAN_BUTTON_DECREMENT_PIN, fanDecrement);

auto ledStrip = LedStripController<LED_STRIP_PIN, LED_STRIP_LENGTH>();

void ledModeIterate();
auto ledButtonMode = ButtonController(LED_BUTTON_MODE_PIN, ledModeIterate);
void ledIncrement();
auto ledButtonIncrement = ButtonController(LED_BUTTON_INCREMENT_PIN, ledIncrement);
void ledDecrement();
auto ledButtonDecrement = ButtonController(LED_BUTTON_DECREMENT_PIN, ledDecrement);

auto tableHeightController = TableHeightController(TABLE_HEIGHT_RAISE_PIN, TABLE_HEIGHT_LOWER_PIN);

auto ledBuiltin = LedController(LED_BUILTIN);

// LedStrip Animations
const uint8_t fps = 60;
auto ledStripRpmHue = LedStripRpmHue(ledStrip.getLedStrip(), fps, 200);
auto ledStripWhite = LedStripSolidColor(ledStrip.getLedStrip(), CRGB(255, 255, 255));
auto ledStripHueFade = LedStripHueFade(ledStrip.getLedStrip(), fps, 0);

BaseAnimation* ledAnimationPointers[] = {
    &ledStripRpmHue,
    &ledStripWhite,
    &ledStripHueFade};

// Function Implementations
void ledStripOnRpmUpdate(uint16_t rpm)
{
    static uint16_t rpmLerped = rpm;
    rpmLerped = rpmLerped + (rpm >> 4) - (rpmLerped >> 4);
    ledStripRpmHue.setRpm(rpmLerped);

#ifdef DEBUG_PRINT_RPM
    Serial.print("RPM: ");
    Serial.print(rpm);
    Serial.print("; Lerped RPM:");
    Serial.println(rpmLerped);
#endif
}

void fanModeIterate() {
    #ifdef DEBUG_PRINT_BUTTON
        Serial.println("fanModeIterate");
    #endif
}
void fanIncrement()
{
    if(fanButtonMode.isPressed()) {
        tableHeightController.raise(500);
    }
    else {
        uint8_t power = fanController.getPowerTarget();
        if (power < 255 - 32)
            power += 32;
        else
            power = 255;

        fanController.setPower(power);
        ledStrip.displayIndicator(nullptr, power);
        #ifdef DEBUG_PRINT_BUTTON
            Serial.print("FanPowerIncrementTo: ");
            Serial.println(power);
        #endif
    }
}
void fanDecrement()
{    
    if(fanButtonMode.isPressed()) {
        tableHeightController.lower(500);
    }
    else {
        uint8_t power = fanController.getPowerTarget();
        if (power > 32)
            power -= 32;
        else
            power = 0;

        fanController.setPower(power);
        ledStrip.displayIndicator(nullptr, power);

        #ifdef DEBUG_PRINT_BUTTON
            Serial.print("FanPowerDecrementTo: ");
            Serial.println(power);
        #endif
    }
}

void ledModeIterate()
{
    static uint8_t ledMode = 0;
    ledMode++;
    ledMode = ledMode % 3;

#ifdef DEBUG_PRINT_BUTTON
    Serial.print("ledModeIterateTo: ");
    Serial.println(ledMode);
#endif

    ledStrip.setAnimation(ledAnimationPointers[ledMode]);
}
uint8_t brightness = 32;
void ledIncrement()
{
    brightness = min(255, brightness+32);
    ledStrip.setBrightness(brightness*brightness/255);
#ifdef DEBUG_PRINT_BUTTON
    Serial.print("LedPowerIncrementTo: ");
    Serial.println(ledStrip.getBrightness());
#endif
}
void ledDecrement()
{
    brightness = max(0, brightness-32);
    ledStrip.setBrightness(brightness*brightness/255);
#ifdef DEBUG_PRINT_BUTTON
    Serial.print("LedPowerDecrementTo: ");
    Serial.println(ledStrip.getBrightness());
#endif
}

void setup()
{
    ledStrip.setBrightness(brightness);
    ledStrip.setAnimation(&ledStripRpmHue);
    Serial.begin(1000000);
}

void loop()
{
    fanButtonMode.Tick();
    fanButtonIncrement.Tick();
    fanButtonDecrement.Tick();

    ledButtonMode.Tick();
    ledButtonIncrement.Tick();
    ledButtonDecrement.Tick();

    tableHeightController.tick();

    ledStrip.tick();
}
