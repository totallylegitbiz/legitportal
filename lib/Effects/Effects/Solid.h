
void solidEffectLoop(struct EffectDataPacket *effectState, CHSV c)
{

  for (uint16_t i = 0; i < LED_CNT; i++)
  {
    Serial.println("i = ");
    Serial.println(i);
    leds[i] = c;
  }

  copyLedsWithOffset();
}

void solidEffectLoop(struct EffectDataPacket *effectState, CRGB c)
{

  for (uint16_t i = 0; i < LED_CNT; i++)
  {
    leds[i] = c;
  }

  copyLedsWithOffset();
}