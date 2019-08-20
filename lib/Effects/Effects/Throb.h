void throbEffectLoop(struct EffectDataPacket *effectState, int hue, uint16_t loopMsBase = 3000)
{

  const float modifierPercent = (float)effectState->effectModifier / 1024;

  const uint16_t loopMs = loopMsBase - (modifierPercent * loopMsBase);

  const uint16_t loopPosition = effectState->loopPosition % loopMs;
  const float loopPercent = float(loopPosition) / loopMs;
  const uint8_t intensity = abs(cos(PI * .5 * loopPercent * 2) * 255);

  for (uint16_t idx = 0; idx < LED_CNT; idx++)
  {
    leds[idx] = CHSV(hue, 255, intensity);
  }

  copyLedsWithOffset();
}