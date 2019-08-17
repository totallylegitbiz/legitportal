#include <FastLED.h>

void ledStripSetup()
{

  FastLED.addLeds<WS2812, LED_PIN, GRB>(cleds, LED_CNT);
  // FastLED.setBrightness(config.LED_BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(config.MAX_VOLTS, config.MAX_MAMP);

  Serial.print("@@ LEDs max amp: ");
  Serial.println(config.MAX_MAMP);
  FastLED.clear();
}