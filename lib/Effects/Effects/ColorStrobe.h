

void colorStrobeEffectLoop(struct EffectState *effectState)
{

  const CRGB RED = CRGB(255, 0, 0);
  const CRGB BLUE = CRGB(255, 0, 0);

  const uint8_t blinkTs = 20;
  const uint8_t steps = 10;
  const uint16_t loopMs = 1000;

  const uint8_t loopPosition = effectState->loopPosition % loopMs;
  const float loopPercent = float(loopPosition) / loopMs;
  const uint8_t step = loopPercent * steps;
  const uint8_t stepBase = (loopMs / steps) * step;

  const uint8_t hueLow = 170;
  const uint8_t hueHigh = 300;

  const uint8_t hue = hueLow + (hueHigh - hueLow) * loopPercent;

  for (uint16_t i = 0; i < config.LED_CNT; i++)
  {
    if (loopPosition < stepBase + blinkTs)
    {
      leds[i] = CHSV(hue, 220, 255);
    }
    else
    {
      leds[i] = CRGB(0, 0, 0);
    }
  }
  copyLedsWithOffset();
}