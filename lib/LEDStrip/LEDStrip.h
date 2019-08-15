#include <FastLED.h>

void ledStripSetup()
{
  FastLED.addLeds<WS2812, LED_PIN, GRB>(cleds, LED_CNT);
  FastLED.setBrightness(config.LED_BRIGHTNESS);
  FastLED.clear();
}