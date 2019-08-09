#include <FastLED.h>

void ledStripSetup()
{
  const int LED_PIN = config.LED_PIN;

  FastLED.addLeds<WS2812, LED_PIN, GRB>(cleds, config.LED_OFFSET, config.LED_CNT);
  FastLED.setBrightness(config.LED_BRIGHTNESS);
}