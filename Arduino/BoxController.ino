#include "headers/BoxController.h"


#define FAN_READ_PIN 5
#define FAN_WRITE_PIN 6
#define BUTTON_TOGGLE_FANS 2
#define BUTTON_TOGGLE_LIGHT 3
#define LED_STRIP_PIN 11
#define LED_STRIP_LENGTH 50

auto fanController = FanController<FAN_READ_PIN, FAN_WRITE_PIN>(0);
auto ledController = LedController<LED_BUILTIN>();

auto ledStrip = LedStripController<LED_STRIP_PIN, LED_STRIP_LENGTH>();
auto fanButton = ButtonController(BUTTON_TOGGLE_FANS, nullptr, setFanState);
auto lightButton = ButtonController(BUTTON_TOGGLE_LIGHT, switchLightStripIntensity, nullptr);

void setFanState(bool state) {
  if(state) {
    ledController.turnOn();
    fanController.setPower(255);
  }
  else {
    ledController.turnOff();
    fanController.setPower(0);
  }
}
auto fanButton = ButtonController<BUTTON_TOGGLE_FANS, nullptr, setFanState>();

uint8_t lightStripIntencity = 0;
void switchLightStripIntensity() {
  lightStripIntencity += 32;
}
void setLightStripState(bool state) {
  if(state) {
    ledStrip.setColor(1, CRGB(255, 0, 0));
    ledStrip.Show();
  }
  else {
    ledStrip.setColor(1, CRGB(0, 0, 0));
    ledStrip.Show();
  }
}
auto lightButton = ButtonController<BUTTON_TOGGLE_LIGHT, switchLightStripIntensity, nullptr>();



void setup()
{
  Serial.begin(115200);
}

void loop()
{
  static int i = 0;
  i++;
  
  uint16_t rpm = fanController.getRpm();
  Serial.print(i);
  Serial.print(", Rpm: ");
  Serial.println(rpm);

  uint8_t hue = 255*2000/rpm-200;
  CHSV hsv = CHSV(hue, 255, lightStripIntencity);
  for(int i=0; i<LED_STRIP_LENGTH; i++)
    ledStrip.setColor(i, CRGB(hsv));
  ledStrip.Show();

}
