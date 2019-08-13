#include <FastLED.h>

// enum Pins
// {
//   DataPin = 8,
//   ClockPin = 12
// };

// struct PixelUtil
// {

//   template <uint8_t DATA_PIN = DataPin, uint8_t CLOCK_PIN = ClockPin>
//   void init(uint16_t num_pixels)
//   {
//     FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(new CRGB[num_pixels], num_pixels);
//   }
// };

// PixelUtil p;

// void setup()
// {

//   //Using the defaults:
//   p.init(40);

//   //Or the equivalent:
//   p.init<8, 12>(40);
// }

void loop() {}

void ledStripSetup()
{
  const int LED_PIN = config.LED_PIN;

  FastLED.addLeds<WS2812, LED_PIN, GRB>(cleds, config.LED_CNT);
  FastLED.setBrightness(config.LED_BRIGHTNESS);
}