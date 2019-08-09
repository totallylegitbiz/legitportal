
void patternLoopEffectLoop(struct EffectState *effectState, unsigned int stepDelays[], const CHSV stepColors[], unsigned int stepCnt)
{
  unsigned int loopMs = 0;
  unsigned int step = 0;

  for (unsigned int i = 0; i < stepCnt; i++)
  {
    loopMs += stepDelays[i];
  }

  const unsigned int loopPosition = effectState->loopPosition % loopMs;

  unsigned int loopCnt = 0;

  for (unsigned int i = 0; i < stepCnt; i++)
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