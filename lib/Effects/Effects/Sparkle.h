
void sparkleEffectLoop(struct EffectDataPacket *effectState, uint8_t sat)
{

  const uint8_t SPARKLE_BATCH_CNT = LED_CNT;

  zeroOutStrip();
  for (uint8_t i = 0; i < SPARKLE_BATCH_CNT; i++)
  {
    const uint16_t seed = effectState->loopPosition * i;

    const uint16_t idx = notRandom(0, LED_CNT - 1, seed);
    const uint8_t hue = notRandom(0, 255, seed);

    leds[idx] = CHSV(hue, sat, 255);
  }
}
