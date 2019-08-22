
void patternLoopEffectLoop(struct EffectDataPacket *effectState, uint16_t stepDelays[], const CHSV stepColors[], uint8_t stepCnt)
{
  uint16_t loopMs = 0;
  uint8_t step = 0;

  for (uint8_t i = 0; i < stepCnt; i++)
  {
    loopMs += stepDelays[i];
  }

  const uint16_t loopPosition = effectState->loopPosition % loopMs;

  uint16_t loopCnt = 0;

  for (uint8_t i = 0; i < stepCnt; i++)
  {
    loopCnt += stepDelays[i];
    if (loopPosition > loopCnt)
    {
      step++;
    }
  }

  for (uint16_t i = 0; i < LED_CNT; i++)
  {
    leds[i] = stepColors[step];
  }
}