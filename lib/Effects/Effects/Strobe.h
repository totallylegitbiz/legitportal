#include <EffectTypes.h>

void strobeEffectLoop(struct EffectState *effectState)
{
    const unsigned int loopMs = 50;
    const unsigned int loopPosition = effectState->loopPosition % loopMs;

    const int COLOR_ON = 0x666666;
    const int COLOR_OFF = 0x00000;

    for (int i = 0; i < LED_CNT; i++)
    {
        if (loopPosition < loopMs / 2)
        {
            leds[i] = COLOR_OFF;
        }
        else
        {
            leds[i] = COLOR_ON;
        }
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