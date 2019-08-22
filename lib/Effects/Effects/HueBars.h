void hueBarsEffectLoop(struct EffectDataPacket *effectState, uint16_t loopMs, uint16_t hueLoopMs, uint8_t width)
{
  const float loopPercent = (float)(effectState->loopPosition % loopMs) / loopMs;
  const float hueLoopPercent = (float)(effectState->loopPosition % hueLoopMs) / hueLoopMs;

  const uint8_t lines = ceil(LED_CNT / 30);
  const uint16_t offset = loopPercent * LED_CNT;

  zeroOutStrip();

  for (uint16_t i = 0; i < lines; i++)
  {
    const float linePercent = float(i) / lines;
    const uint8_t baseIdx = (int((linePercent)*LED_CNT) + offset) % 255;

    drawDash(width, baseIdx, CHSV(255 * hueLoopPercent, 255, 255));
  }
}
