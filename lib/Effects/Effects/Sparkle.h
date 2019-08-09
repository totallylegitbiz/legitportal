RunEvery runEvery(20);

void sparkleEffectLoop(struct EffectState *effectState, int sat)
{

  const int SPARKLE_BATCH_CNT = config.LED_CNT / 2;

  const unsigned int loopMs = 60 * 1000;
  const unsigned int loopPosition = effectState->loopPosition % loopMs;

  if (runEvery.shouldRun())
  {
    for (int i = 0; i < SPARKLE_BATCH_CNT; i++)
    {
      const int idx = notRandom(0, config.LED_CNT - 1, loopPosition * i);
      const int hue = notRandom(0, 255, loopPosition * i);
      leds[idx] = CHSV(hue, sat, 255);
    }

    for (int i = 0; i < SPARKLE_BATCH_CNT / 3; i++)
    {
      const int blankIdx = notRandom(0, (config.LED_CNT - 1) * 5, loopPosition * i);

      leds[blankIdx % config.LED_CNT] = CHSV(0, 255, 0);
    }
  }

  copyLedsWithOffset();
}