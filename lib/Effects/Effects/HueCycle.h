void hueCycleEffectLoop(struct EffectDataPacket *effectState, uint32_t loopMs, uint8_t hue1, uint8_t hue2)
{
  const uint32_t loopPosition = effectState->loopPosition % loopMs;
  const float loopPercent = float(loopPosition) / loopMs;

  // const uint8_t hue = hue1 + (float(hue2 - hue1) * loopPercent);

  const uint8_t hue = hue1 + (float(hue2 - hue1) * abs((loopPercent / .5) - 1.0));

  for (uint16_t i = 0; i < LED_CNT; i++)
  {
    leds[i] = CHSV(hue, 255, 255);
  }
}