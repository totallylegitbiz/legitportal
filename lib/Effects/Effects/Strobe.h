#include <EffectTypes.h>

void strobeEffectLoop(struct EffectState *effectState, CRGB colorOff, CRGB colorOn, int loopMs)
{
    const unsigned int loopPosition = effectState->loopPosition % loopMs;

    for (int i = 0; i < LED_CNT; i++)
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
}

// void strobeEffectLoop(struct EffectState *effectState)
// {
//     const unsigned int loopMs = 10000;
//     const unsigned int loopPosition = effectState->loopPosition % loopMs;

//     const float loopPercent = float(loopPosition) / loopMs;

//     const int hue = floor((loopPercent * 255) / 5) * 5;

//     for (int i = 0; i < LED_CNT; i++)
//     {
//         leds[i] = CHSV(hue, 255, 50);
//     }
// }