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

// Function Implementations
void ledStripOnRpmUpdate(uint16_t rpm)
{
    static uint16_t rpmLerped = rpm;
    rpmLerped = rpmLerped + (rpm >> 3) - (rpmLerped >> 3);

    #ifdef DEBUG_PRINT_RPM
        Serial.print("RPM: ");
        Serial.print(rpm);
        Serial.print("; Lerped RPM:");
        Serial.println(rpmLerped);
    #endif

    uint8_t hue = map(rpmLerped, 200, 1900, 0, 255);
    CHSV hsv = CHSV(hue, 255, 255);

    ledStrip.setColor(CRGB(hsv));
    ledStrip.Show();
}

void fanModeIterate() {}
void fanIncrement()
{
    uint8_t currentPower = fanController.getPowerTarget();
    if (currentPower < 255 - 32)
        fanController.setPower(currentPower + 32);
    else
        fanController.setPower(255);
#ifdef DEBUG_PRINT_BUTTON
    Serial.print("FanPowerIncrementTo: ");
    Serial.println(fanController.getPowerTarget());
#endif
}
void fanDecrement()
{
    uint8_t currentPower = fanController.getPowerTarget();
    if (currentPower > 32)
        fanController.setPower(currentPower - 32);
    else
        fanController.setPower(0);
#ifdef DEBUG_PRINT_BUTTON
    Serial.print("FanPowerDecrementTo: ");
    Serial.println(fanController.getPowerTarget());
#endif
}

void ledModeIterate() {}
void ledIncrement()
{
    uint8_t currentPower = ledStrip.getBrightness();
    if (currentPower < 255 - 32)
        ledStrip.setBrightness(currentPower + 32);
    else
        ledStrip.setBrightness(255);
#ifdef DEBUG_PRINT_BUTTON
    Serial.print("LedPowerIncrementTo: ");
    Serial.println(ledStrip.getBrightness());
#endif
}
void ledDecrement()
{
    uint8_t currentPower = ledStrip.getBrightness();
    if (currentPower > 32)
        ledStrip.setBrightness(currentPower - 32);
    else
        ledStrip.setBrightness(0);
#ifdef DEBUG_PRINT_BUTTON
    Serial.print("LedPowerDecrementTo: ");
    Serial.println(ledStrip.getBrightness());
#endif
}

void setup()
{
    ledStrip.setBrightness(32);
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
}
