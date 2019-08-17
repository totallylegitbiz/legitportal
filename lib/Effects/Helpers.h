
void drawDash(int width, uint16_t idx, CRGB color)
{

  for (int i = (-width / 2); i < (width / 2); i++)
  {
    const uint16_t currIdx = (LED_CNT + idx + i) % LED_CNT;

    leds[currIdx] += color;
  }
}

void drawDashWithoutAdd(int width, uint16_t idx, CRGB color)
{

  for (int i = (-width / 2); i < (width / 2); i++)
  {
    const uint16_t currIdx = (LED_CNT + idx + i) % LED_CNT;

    leds[currIdx] = color;
  }
}

/* fade() 0-255 amount to keep (255 changes nothing) */
void fadeDown(uint8_t by)
{
  for (uint16_t i = 0; i < LED_CNT; i++)
  {
    leds[i].subtractFromRGB(by);
  }
}
void fadeUp(uint8_t by)
{
  for (uint16_t i = 0; i < LED_CNT; i++)
  {
    leds[i].addToRGB(by);
  }
}

int notRandom(uint16_t from, uint16_t to, uint16_t seed)
{

  randomSeed(seed);
  const uint16_t rand = random(from, to);
  randomSeed(analogRead(UNUSED_PIN));

  return rand;
}

int yesRandom(uint16_t from, uint16_t to)
{
  reseedRandom();
  return random(from, to);
}

// void zeroOutOutputStrip()
// {
//   for (uint16_t i = 0; i < LED_CNT + LED_OFFSET; i++)
//   {
//     cleds[i] = CRGB(0, 0, 0);
//   }
// }

void zeroOutStrip()
{
  FastLED.clear();
}

void colorOutStrip(CRGB color)
{
  for (uint16_t i = 0; i < LED_CNT; i++)
  {
    leds[i] = color;
  }
}

void colorOutStrip(CHSV color)
{
  for (uint16_t i = 0; i < LED_CNT; i++)
  {
    leds[i] = color;
  }
}

void copyLedsWithOffset()
{
  // for (uint16_t i = 0; i < LED_CNT; i++)
  // {
  //   cleds[i + LED_OFFSET] = leds[i];
  // }
}

// void copyLedsWithOffsetGamma()
// {
//   for (uint16_t i = 0; i < LED_CNT; i++)
//   {
//     const int idx = i + LED_OFFSET;
//     cleds[idx].r = dim8_video(leds[idx].r);
//     cleds[idx].g = dim8_video(leds[idx].g);
//     cleds[idx].b = dim8_video(leds[idx].b);
//   }
// }

class RunEvery
{
public:
  RunEvery(uint8_t everyMs);
  boolean shouldRun();

private:
  uint8_t _everyMs;
  uint32_t _lastRunMs;
};

RunEvery::RunEvery(uint8_t everyMs)
{
  _everyMs = everyMs;
  _lastRunMs = 0;
}

bool RunEvery::shouldRun()
{
  if (_lastRunMs + _everyMs < millis())
  {
    _lastRunMs = millis() - (millis() % _everyMs);
    return true;
  }

  return false;
}

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char *sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif // __arm__

int freeMemory()
{
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char *>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif // __arm__
}