#include <FastLED.h>
#include <Config.h>

CRGB cleds[LED_CNT + LED_OFFSET];

void ledStripSetup()
{
    FastLED.addLeds<WS2812, LED_PIN, GRB>(cleds, LED_CNT + LED_OFFSET);
    FastLED.setBrightness(LED_BRIGHTNESS);
}