#include <EffectTypes.h>

void spinEffectLoop(struct EffectState *effectState)
{

    // Time for this effect to loop;

    const unsigned int loopMs = 1000;
    const unsigned int loopPosition = effectState->loopPosition % loopMs;

    // const float loopPerfect = float(effectState.loopPosition) / EFFECT_LOOP_MS;

    const int ledOffset = LED_CNT * (float(loopPosition) / loopMs);

    for (int i = 0; i < LED_CNT; i++)
    {
        leds[i] = CHSV(0, 0, 0);
    }

    leds[ledOffset] = CHSV(0, 0, 100);
}