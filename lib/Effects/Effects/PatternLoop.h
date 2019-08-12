
void patternLoopEffectLoop(struct EffectState *effectState, uint8_t stepDelays[], const CHSV stepColors[], uint8_t stepCnt)
{
  uint16_t loopMs = 0;
  uint8_t step = 0;

  for (uint8_t i = 0; i < stepCnt; i++)
  {
    loopMs += stepDelays[i];
  }

  const uint8_t loopPosition = effectState->loopPosition % loopMs;

  uint8_t loopCnt = 0;

  for (uint8_t i = 0; i < stepCnt; i++)
  {
    loopCnt += stepDelays[i];
    if (loopPosition > loopCnt)
    {
      step++;
    }
  }

  for (int i = 0; i < config.LED_CNT; i++)
  {
    leds[i] = stepColors[step];
  }

  copyLedsWithOffset();
}