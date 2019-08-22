#include <FastLED.h>

void ledStripSetup()
{

  SERIAL_PRINTLN("Setting up LED Strip...");
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_CNT);
  // FastLED.setBrightness(config.LED_BRIGHTNESS);

  FastLED.setMaxPowerInVoltsAndMilliamps(config.MAX_VOLTS, config.MAX_MAMP);

  SERIAL_PRINT("# LEDs max amp: ");
  SERIAL_PRINTLN(config.MAX_MAMP);
  FastLED.clear();
}