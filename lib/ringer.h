#include <FastLED.h>

extern class Ringer
{
public:
  void setup()
  {
    Serial.print("set...");
  }

  void loop(CRGB cleds[NUM_LEDS], float basePercent)
  {

    float widthPercent = .15;
    int dashNum = 4;

    int dashOffset = dashNum;
    float ledWidth = widthPercent * NUM_LEDS;

    for (int i = 0; i < NUM_LEDS; i++)
    {
      cleds[i] = CHSV(0, 255, 255);
    }
  }
};
