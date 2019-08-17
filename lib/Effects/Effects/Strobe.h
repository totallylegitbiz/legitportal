

void strobeEffectLoop(struct EffectDataPacket *effectState, CRGB colorOff, CRGB colorOn, uint16_t loopMs)
{
  const uint8_t loopPosition = effectState->loopPosition % loopMs;

  for (uint16_t i = 0; i < LED_CNT; i++)
  {
    if (loopPosition < loopMs / 2)
    {
      leds[i] = colorOff;
    }
    else
    {
      leds[i] = colorOn;
    }
  }
  copyLedsWithOffset();
}