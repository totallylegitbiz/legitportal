void hueSpinEffectLoop(struct EffectDataPacket *effectState, uint16_t loopMs)
{
  const uint8_t loopPosition = effectState->loopPosition % loopMs;
  const float loopPercent = float(loopPosition) / loopMs;

  const int loopOffset = loopPercent * config.LED_CNT;

  for (uint16_t i = 0; i < config.LED_CNT; i++)
  {
    const float ledPercent = float(i) / config.LED_CNT;
    const uint8_t hueOffset = ledPercent * 255;
    const uint8_t idx = (i + loopOffset) % config.LED_CNT;
    leds[idx] = CHSV(hueOffset, 255, 255);
  }

  copyLedsWithOffset();
}