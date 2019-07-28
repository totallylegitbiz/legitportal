#include <EffectTypes.h>

// void strobeEffectLoop(struct EffectState *effectState)
// {
//     const unsigned int loopMs = 500;
//     const unsigned int loopPosition = effectState->loopPosition % loopMs;

//     const CRGB COLOR_ON = CRGB(30, 0, 0);
//     const CRGB COLOR_OFF = CRGB(30, 30, 0);

//     for (int i = 0; i < LED_CNT; i++)
//     {
//         if (loopPosition < loopMs / 2)
//         {
//             leds[i] = COLOR_OFF;
//         }
//         else
//         {
//             leds[i] = COLOR_ON;
//         }
//     }
// }

void strobeEffectLoop(struct EffectState *effectState)
{
    const unsigned int loopMs = 10000;
    const unsigned int loopPosition = effectState->loopPosition % loopMs;

    const float loopPercent = float(loopPosition) / loopMs;

    const int hue = floor((loopPercent * 255) / 5) * 5;

    for (int i = 0; i < LED_CNT; i++)
    {
        leds[i] = CHSV(hue, 255, 50);
    }
}