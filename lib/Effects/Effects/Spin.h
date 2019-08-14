void spinEffectLoop(struct EffectDataPacket *effectState, uint16_t loopMs)
{
  const uint16_t loopPosition = effectState->loopPosition % loopMs;
  const uint16_t ledOffset = LED_CNT * (float(loopPosition) / loopMs);

  zeroOutStrip();

  const int lines = 2;
  const int lineWidth = LED_CNT / 7;
  const int lineIdx = LED_CNT / lines;

  for (int i = 0; i < lines; i++)
  {

    const int offset = ledOffset + (i * lineIdx);

    if (i % 2)
    {
      drawDashWithoutAdd(lineWidth, offset, CHSV(i * (255 / lines), 255, 255));
    }
    else
    {
      drawDashWithoutAdd(lineWidth, LED_CNT - offset, CHSV(i * (255 / lines), 255, 255));
    }
  }

  copyLedsWithOffset();
}