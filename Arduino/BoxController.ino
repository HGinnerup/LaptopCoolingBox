#include "headers/BoxController.h"

#define DEBUG

#define FAN_READ_PIN 2
#define FAN_WRITE_PIN 3

#define LED_STRIP_PIN 5
#define LED_STRIP_LENGTH 23
#define LED_BUTTON_MODE_PIN 7
#define LED_BUTTON_INCREMENT_PIN 6
#define LED_BUTTON_DECREMENT_PIN 8

#define FAN_BUTTON_MODE_PIN 10
#define FAN_BUTTON_INCREMENT_PIN 9
#define FAN_BUTTON_DECREMENT_PIN 11

#ifdef DEBUG
#define DEBUG_PRINT_BUTTON
#define DEBUG_PRINT_RPM
#endif

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

void fanModeIterate() {}
void fanIncrement()
{
    uint8_t power = fanController.getPowerTarget();
    if (power < 255 - 32)
        power += 32;
    else
        power = 255;

    fanController.setPower(power);
    ledStrip.displayIndicator(CRGB(0, 255, 0), power);

#ifdef DEBUG_PRINT_BUTTON
    Serial.print("FanPowerIncrementTo: ");
    Serial.println(power);
#endif
}
void fanDecrement()
{
    uint8_t power = fanController.getPowerTarget();
    if (power > 32)
        power -= 32;
    else
        power = 0;

    fanController.setPower(power);
    ledStrip.displayIndicator(CRGB(0, 255, 0), power);

#ifdef DEBUG_PRINT_BUTTON
    Serial.print("FanPowerDecrementTo: ");
    Serial.println(power);
#endif
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
void ledIncrement()
{
    ledStrip.increaseBrightness(32);
#ifdef DEBUG_PRINT_BUTTON
    Serial.print("LedPowerIncrementTo: ");
    Serial.println(ledStrip.getBrightness());
#endif
}
void ledDecrement()
{
    ledStrip.decreaseBrightness(32);
#ifdef DEBUG_PRINT_BUTTON
    Serial.print("LedPowerDecrementTo: ");
    Serial.println(ledStrip.getBrightness());
#endif
}

void setup()
{
    ledStrip.setBrightness(32);
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

    ledStrip.tick();
}
