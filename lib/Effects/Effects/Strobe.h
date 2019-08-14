

void strobeEffectLoop(struct EffectDataPacket *effectState, CRGB colorOff, CRGB colorOn, uint16_t loopMs)
{
  const uint8_t loopPosition = effectState->loopPosition % loopMs;

  for (uint16_t i = 0; i < LED_CNT; i++)
  {
    if (loopPosition < loopMs / 2)
    {
      leds[i] = colorOff;
    }
    else
    {
      leds[i] = colorOn;
    }
  }
  copyLedsWithOffset();
}

// void strobeEffectLoop(struct EffectDataPacket *effectState)
// {
//     const uint16_t loopMs = 10000;
//     const uint8_t loopPosition = effectState->loopPosition % loopMs;

//     const float loopPercent = float(loopPosition) / loopMs;

//     const int hue = floor((loopPercent * 255) / 5) * 5;

//     for (int i = 0; i < LED_CNT; i++)
//     {
//         leds[i] = CHSV(hue, 255, 50);
//     }
// }