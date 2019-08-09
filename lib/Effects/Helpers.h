
void drawDash(int width, uint16_t idx, CRGB color)
{

  for (int i = (-width / 2); i < (width / 2); i++)
  {
    const uint16_t currIdx = (config.LED_CNT + idx + i) % config.LED_CNT;

    leds[currIdx] += color;
  }
}

void drawDashWithoutAdd(int width, uint16_t idx, CRGB color)
{

  for (int i = (-width / 2); i < (width / 2); i++)
  {
    const uint16_t currIdx = (config.LED_CNT + idx + i) % config.LED_CNT;

    leds[currIdx] = color;
  }
}

/* fade() 0-255 amount to keep (255 changes nothing) */
void fadeDown(unsigned int by)
{
  for (int i = 0; i < config.LED_CNT; i++)
  {
    leds[i].subtractFromRGB(by);
  }
}
void fadeUp(unsigned int by)
{
  for (int i = 0; i < config.LED_CNT; i++)
  {
    leds[i].addToRGB(by);
  }
}

int notRandom(int from, int to, int seed)
{

  randomSeed(seed);
  const int rand = random(from, to);
  randomSeed(analogRead(0));

  return rand;
}

void zeroOutOutputStrip()
{
  for (int i = 0; i < config.LED_CNT + config.LED_OFFSET; i++)
  {
    cleds[i] = CRGB(0, 0, 0);
  }
}

void zeroOutStrip()
{
  for (int i = 0; i < config.LED_CNT; i++)
  {
    leds[i] = CRGB(0, 0, 0);
  }
}

void colorOutStrip(CRGB color)
{
  for (int i = 0; i < config.LED_CNT; i++)
  {
    leds[i] = color;
  }
}

void colorOutStrip(CHSV color)
{
  for (int i = 0; i < config.LED_CNT; i++)
  {
    leds[i] = color;
  }
}

void copyLedsWithOffset()
{
  for (int i = 0; i < config.LED_CNT; i++)
  {
    cleds[i + config.LED_OFFSET] = leds[i];
  }
}

void copyLedsWithOffsetGamma()
{
  for (int i = 0; i < config.LED_CNT; i++)
  {
    const int idx = i + config.LED_OFFSET;
    cleds[idx].r = dim8_video(leds[idx].r);
    cleds[idx].g = dim8_video(leds[idx].g);
    cleds[idx].b = dim8_video(leds[idx].b);
  }
}

class RunEvery
{
public:
  RunEvery(unsigned int everyMs);
  boolean shouldRun();

private:
  unsigned int _everyMs;
  unsigned long _lastRunMs;
};

RunEvery::RunEvery(unsigned int everyMs)
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
