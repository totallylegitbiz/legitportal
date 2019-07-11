#include <FastLED.h>

extern class Ringer
{
public:
  void setup()
  {
    Serial.print("set...");
  }

  void loop(CRGB cleds[LED_NUM], float basePercent)
  {

    float widthPercent = .15;
    int dashNum = 4;

    int dashOffset = dashNum;
    float ledWidth = widthPercent * LED_NUM;

    for (int i = 0; i < LED_NUM; i++)
    {
      cleds[i] = CHSV(0, 255, 255);
    }
  }
};
