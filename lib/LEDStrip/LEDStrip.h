#include <FastLED.h>
#include <Config.h>

void ledStripSetup()
{
  FastLED.addLeds<WS2812, config.LED_PIN, GRB>(cleds, MAX_LEDS);
  FastLED.setBrightness(config.LED_BRIGHTNESS);
}