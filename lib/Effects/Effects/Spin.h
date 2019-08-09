

void spinEffectLoop(struct EffectState *effectState, uint8_t loopMs)
{
  const uint16_t loopPosition = effectState->loopPosition % loopMs;
  const uint16_t ledOffset = config.LED_CNT * (float(loopPosition) / loopMs);

  // zeroOutStrip();

  // colorOutStrip(CRGB(255,255,255));

  // Red blue dash
  // drawDash(LED_CNT/3, ledOffset, CRGB(0, 220, 255));
  // drawDash(LED_CNT/3, LED_CNT - ledOffset, CRGB(255, 0, 0));

  // Red blue dash

  const int lines = 5;
  const int lineWidth = config.LED_CNT / 7;
  const int lineIdx = config.LED_CNT / lines;

  for (int i = 0; i < lines; i++)
  {

    const int offset = ledOffset + (i * lineIdx);

    if (i % 2)
    {
      drawDashWithoutAdd(lineWidth, offset, CHSV(i * (255 / lines), 255, 255));
    }
    else
    {
      drawDashWithoutAdd(lineWidth, config.LED_CNT - offset, CHSV(i * (255 / lines), 255, 255));
    }
  }

  for (int i = 0; i < lines; i++)
  {

    const int offset = ledOffset + (i * lineIdx);

    if (i % 2)
    {
      drawDash(lineWidth, offset, CHSV(i * (255 / lines), 255, 255));
    }
    else
    {
      drawDash(lineWidth, config.LED_CNT - offset, CHSV(i * (255 / lines), 255, 255));
    }
  }

  // drawDash(lineWidth, ledOffset, CHSV(0, 255, 255));
  // drawDash(lineWidth, LED_CNT - ledOffset, CHSV(85, 255, 255));
  // drawDash(lineWidth, ledOffset + lineIdx, CHSV(171, 255, 255));

  copyLedsWithOffset();
}