void dashSpinEffectLoop(struct EffectDataPacket *effectState, int16_t loopMs, bool hueLoop = false)
{
  const int8_t dashWidth = 5;
  const int8_t colorCnt = 50;

  const int16_t loopPosition = effectState->loopPosition % loopMs;

  const uint8_t hueBase = effectState->transmitterId % 255;

  uint8_t hue = 0;

  if (hueLoop)
  {
    hue = (float)loopPosition / loopMs * 255;
  }

  solidEffectLoop(effectState, CHSV(hue, 255, 255));

  const int8_t dashOffset = ((float)loopPosition / loopMs) * dashWidth;

  for (int8_t i = 0; i < LED_CNT; i++)
  {
    if (i % dashWidth == dashOffset)
    {
      leds[i] = CHSV((hue + hueBase) % 255, 255, 20);
    }
  }
}