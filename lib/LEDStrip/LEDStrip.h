#include <FastLED.h>

void ledStripSetup()
{
  Serial.print("LED_PIN: ");
  Serial.println(LED_PIN);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(cleds, LED_CNT);
  FastLED.setBrightness(config.LED_BRIGHTNESS);
}