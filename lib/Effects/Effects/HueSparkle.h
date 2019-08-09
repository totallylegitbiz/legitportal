
#include <EffectTypes.h>

void hueSparkleEffectLoop(struct EffectState *effectState)
{
  const unsigned int loopMs = 60 * 1000;
  const unsigned int loopPosition = effectState->loopPosition % loopMs;
  const float loopPercent = float(loopPosition) / loopMs;

  const unsigned int hue1 = 128;
  const unsigned int hue2 = 224;

  for (int i = 0; i < LED_CNT; i++)
  {
    const float ledPercent = float(i) / LED_CNT;
    const unsigned int hue = hue1 + (float(hue2 - hue1) * abs((ledPercent / .5) - 1.0));
    const unsigned int idx = i + (loopPercent * LED_CNT);

    leds[idx % LED_CNT] = CHSV(hue, 255, 255);
  }

  copyLedsWithOffset();
}