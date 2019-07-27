#include <EffectTypes.h>

void spinEffectLoop(struct EffectState *effectState)
{

    // Time for this effect to loop;

    const unsigned int loopMs = 500;
    const unsigned int loopPosition = effectState->loopPosition % loopMs;

    // const float loopPerfect = float(effectState.loopPosition) / EFFECT_LOOP_MS;

    const int ledOffset = LED_CNT * (float(loopPosition) / loopMs);

    for (int i = 0; i < LED_CNT; i++)
    {
        // const int idx = (i + ledOffset) % LED_CNT;
        // const int hue = (i * 255) / LED_CNT;
        // const int hue = (i * 255) / LED_CNT;
        leds[i] = CHSV(100, 255, 100);
    }

    leds[ledOffset] = CHSV(0, 0, 100);
}