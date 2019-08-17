void hueBarsEffectLoop(struct EffectDataPacket *effectState, uint16_t loopMs)
{

  const float loopPercent = float(effectState->loopPosition % loopMs) / loopMs;
  // const float huePercent = float(effectState->loopPosition) / config.EFFECT_LOOP_MS;

  const uint8_t lines = ceil(LED_CNT / 30);

  const int offset = loopPercent * LED_CNT;

  // zeroOutStrip();
  for (uint16_t i = 0; i < lines; i++)
  {
    const float linePercent = float(i) / lines;
    const int baseIdx = (int((linePercent)*LED_CNT) + offset) % 255;

    drawDash(2, baseIdx, CHSV(20, 255, 255));
  }

  copyLedsWithOffset();
}
