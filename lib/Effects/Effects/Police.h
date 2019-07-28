#include <EffectTypes.h>

const CRGB RED = CRGB(255, 0, 0);
const CRGB BLUE = CRGB(255, 0, 0);
const int BLINK_CNT = 2;

void policeEffectLoop(struct EffectState *effectState)
{
    const unsigned int loopMs = 1000;
    const unsigned int loopPosition = effectState->loopPosition % loopMs;

    for (int i = 0; i < LED_CNT; i++)
    {
        leds[i] = BlackLightFluorescent;
    }

    // const float loopPerfect = float(effectState.loopPosition) / EFFECT_LOOP_MS;

    // const int ledOffset = LED_CNT * (float(loopPosition) / loopMs);

    // for (int i = 0; i < LED_CNT; i++)
    // {
    //     // const int idx = (i + ledOffset) % LED_CNT;
    //     // const int hue = (i * 255) / LED_CNT;
    //     // const int hue = (i * 255) / LED_CNT;
    //     leds[i] = CHSV(100, 255, 100);
    // }

    // leds[ledOffset] = CHSV(0, 0, 100);
}