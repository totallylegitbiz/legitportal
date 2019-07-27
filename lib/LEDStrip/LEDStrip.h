#include <FastLED.h>
#include <Config.h>

CRGB leds[LED_CNT];

void ledStripSetup()
{
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_CNT);
}