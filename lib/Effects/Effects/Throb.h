

void throbEffectLoop(struct EffectState *effectState, int hue)
{
  const uint16_t loopMs = 3000;
  const uint8_t loopPosition = effectState->loopPosition % loopMs;
  const float loopPercent = float(loopPosition) / loopMs;
  const uint8_t intensity = abs(cos(PI * .5 * loopPercent * 2) * 255);

  for (uint16_t i = 0; i < config.LED_CNT; i++)
  {
    leds[i] = CHSV(hue, 255, intensity);
  }
  copyLedsWithOffset();
}