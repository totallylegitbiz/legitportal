
void solidEffectLoop(struct EffectState *effectState, CHSV c)
{

  for (int i = 0; i < config.LED_CNT; i++)
  {
    leds[i] = c;
  }

  copyLedsWithOffset();
}

void solidEffectLoop(struct EffectState *effectState, CRGB c)
{

  for (int i = 0; i < config.LED_CNT; i++)
  {
    leds[i] = c;
  }

  copyLedsWithOffset();
}