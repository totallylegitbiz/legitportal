#include <FastLED.h>

void ledStripSetup()
{

  Serial.println("Setting up LED Strip...");
  FastLED.addLeds<WS2812, LED_PIN, GRB>(cleds, LED_CNT);
  // FastLED.setBrightness(config.LED_BRIGHTNESS);

  FastLED.setMaxPowerInVoltsAndMilliamps(config.MAX_VOLTS, 100);

  Serial.print("# LEDs max amp: ");
  Serial.println(config.MAX_MAMP);
  FastLED.clear();
}