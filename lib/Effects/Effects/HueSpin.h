#include <EffectTypes.h>

void hueSpinEffectLoop(struct EffectState *effectState, unsigned int loopMs)
{
    const unsigned int loopPosition = effectState->loopPosition % loopMs;
    const float loopPercent = float(loopPosition) / loopMs;

    const int loopOffset = loopPercent * LED_CNT;

    for (int i = 0; i < LED_CNT; i++)
    {
        const float ledPercent = float(i) / LED_CNT;
        const unsigned int hueOffset = ledPercent * 255;
        const unsigned int idx = (i + loopOffset) % LED_CNT;
        leds[idx] = CHSV(hueOffset, 255, 255);
    }

      copyLedsWithOffset();
      
}