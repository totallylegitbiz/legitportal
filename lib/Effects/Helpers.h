
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
void fadeDown(unsigned int by)
{
  for (int i = 0; i < LED_CNT; i++)
  {
    leds[i].subtractFromRGB(by);
  }
}
void fadeUp(unsigned int by)
{
  for (int i = 0; i < LED_CNT; i++)
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