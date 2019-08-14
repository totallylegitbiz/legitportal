

void hueSparkleEffectLoop(struct EffectDataPacket *effectState)
{
  const uint16_t loopMs = 60 * 1000;
  const uint8_t loopPosition = effectState->loopPosition % loopMs;
  const float loopPercent = float(loopPosition) / loopMs;

  const uint8_t hue1 = 128;
  const uint8_t hue2 = 224;

  for (int i = 0; i < LED_CNT; i++)
  {
    const float ledPercent = float(i) / LED_CNT;
    const uint8_t hue = hue1 + (float(hue2 - hue1) * abs((ledPercent / .5) - 1.0));
    const uint8_t idx = i + (loopPercent * LED_CNT);

    leds[idx % LED_CNT] = CHSV(hue, 255, 255);
  }

  copyLedsWithOffset();
}