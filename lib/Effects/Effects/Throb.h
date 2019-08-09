#include <EffectTypes.h>

void throbEffectLoop(struct EffectState *effectState, int hue)
{
  const unsigned int loopMs = 3000;
  const unsigned int loopPosition = effectState->loopPosition % loopMs;
  const float loopPercent = float(loopPosition) / loopMs;
  const unsigned int intensity = abs(cos(PI * .5 * loopPercent * 2) * 255);

  for (int i = 0; i < config.LED_CNT; i++)
  {
    leds[i] = CHSV(hue, 255, intensity);
  }
  copyLedsWithOffset();
}