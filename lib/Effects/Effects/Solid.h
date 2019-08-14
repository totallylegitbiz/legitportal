
void solidEffectLoop(struct EffectDataPacket *effectState, CHSV c)
{

  for (uint16_t i = 0; i < config.LED_CNT; i++)
  {
    leds[i] = c;
  }

  copyLedsWithOffset();
}

void solidEffectLoop(struct EffectDataPacket *effectState, CRGB c)
{

  for (uint16_t i = 0; i < config.LED_CNT; i++)
  {
    leds[i] = c;
  }

  copyLedsWithOffset();
}