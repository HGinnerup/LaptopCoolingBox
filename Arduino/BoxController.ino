#include "headers/BoxController.h"

#define FAN_READ_PIN 3
#define FAN_WRITE_PIN 4
#define BUTTON_TOGGLE_FANS 5
#define BUTTON_TOGGLE_LIGHT 6
#define LED_STRIP_PIN 7
#define LED_STRIP_LENGTH 50

auto ledStrip = LedStripController<LED_STRIP_PIN, LED_STRIP_LENGTH>();
void ledStripOnRpmUpdate(uint16_t rpm);
auto fanController = FanController<FAN_READ_PIN, FAN_WRITE_PIN, ledStripOnRpmUpdate>(0);
auto ledController = LedController(LED_BUILTIN);
void setFanState(bool state);
auto fanButton = ButtonController(BUTTON_TOGGLE_FANS, nullptr, setFanState);
void switchLightStripIntensity();
auto lightButton = ButtonController(BUTTON_TOGGLE_LIGHT, switchLightStripIntensity, nullptr);

uint8_t lightStripIntencity = 64;
void ledStripOnRpmUpdate(uint16_t rpm)
{
  static uint16_t rpmLerped = rpm;
  rpmLerped = rpmLerped + (rpm >> 3) - (rpmLerped >> 3);

  uint8_t hue = map(rpmLerped, 200, 1900, 0, 255);
  CHSV hsv = CHSV(hue, 255, lightStripIntencity);
  for (int i = 0; i < LED_STRIP_LENGTH; i++)
    ledStrip.setColor(i, CRGB(hsv));
  ledStrip.Show();

  Serial.print("Rpm: ");
  Serial.println(rpm);
  Serial.print("RpmLerped: ");
  Serial.println(rpmLerped);
}
void setFanState(bool state)
{
  if (state)
  {
    ledController.turnOn();
    fanController.setPower(255);
  }
  else
  {
    ledController.turnOff();
    fanController.setPower(0);
  }
}
void switchLightStripIntensity()
{
  lightStripIntencity += 32;
}

void setup()
{
  Serial.begin(1000000);
}

void loop()
{
  fanButton.Tick();
  lightButton.Tick();
}
