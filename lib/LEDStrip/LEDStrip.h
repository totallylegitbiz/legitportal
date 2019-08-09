#include <FastLED.h>

void ledStripSetup()
{
  FastLED.addLeds<WS2812, config.LED_PIN, GRB>(cleds, config.LED_OFFSET, config.LED_CNT);
  FastLED.setBrightness(config.LED_BRIGHTNESS);
}