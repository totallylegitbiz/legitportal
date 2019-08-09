
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
void fadeDown(uint8_t by)
{
  for (uint16_t i = 0; i < config.LED_CNT; i++)
  {
    leds[i].subtractFromRGB(by);
  }
}
void fadeUp(uint8_t by)
{
  for (uint16_t i = 0; i < config.LED_CNT; i++)
  {
    leds[i].addToRGB(by);
  }
}

int notRandom(uint16_t from, uint16_t to, uint16_t seed)
{
  randomSeed(seed);
  const uint16_t rand = random(from, to);
  randomSeed(analogRead(0));

  return rand;
}

void zeroOutOutputStrip()
{
  for (uint16_t i = 0; i < config.LED_CNT + config.LED_OFFSET; i++)
  {
    cleds[i] = CRGB(0, 0, 0);
  }
}

void zeroOutStrip()
{
  for (uint16_t i = 0; i < config.LED_CNT; i++)
  {
    leds[i] = CRGB(0, 0, 0);
  }
}

void colorOutStrip(CRGB color)
{
  for (uint16_t i = 0; i < config.LED_CNT; i++)
  {
    leds[i] = color;
  }
}

void colorOutStrip(CHSV color)
{
  for (uint16_t i = 0; i < config.LED_CNT; i++)
  {
    leds[i] = color;
  }
}

void copyLedsWithOffset()
{
  for (uint16_t i = 0; i < config.LED_CNT; i++)
  {
    cleds[i + config.LED_OFFSET] = leds[i];
  }
}

void copyLedsWithOffsetGamma()
{
  for (uint16_t i = 0; i < config.LED_CNT; i++)
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
