#include <FastLED.h>

void loop() {}

const int LED_CNT = 100;

void ledStripSetup()
{
  const int LED_PIN = config.LED_PIN;

  FastLED.addLeds<WS2812, LED_PIN, GRB>(cleds, LED_CNT);
  FastLED.setBrightness(config.LED_BRIGHTNESS);
}