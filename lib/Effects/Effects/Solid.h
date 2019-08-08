#include <EffectTypes.h>

void solidEffectLoop(struct EffectState *effectState, CHSV c)
{
  for (int i = 0; i < LED_CNT; i++)
  {
    leds[i] = c;
  }
  
  copyLedsWithOffset();
}