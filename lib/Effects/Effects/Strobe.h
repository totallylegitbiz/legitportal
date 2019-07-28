#include <EffectTypes.h>

void strobeEffectLoop(struct EffectState *effectState)
{
    const unsigned int loopMs = 1000;
    const unsigned int loopPosition = effectState->loopPosition % loopMs;

    const int COLOR_ON = BlackLightFluorescent;
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
}