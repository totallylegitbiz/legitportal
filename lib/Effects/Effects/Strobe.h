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
    const unsigned int loopMs = 1000;
    const unsigned int loopPosition = effectState->loopPosition % loopMs;

    const int avgPosition = (loopPosition / 10) * 10;

    for (int i = 0; i < LED_CNT; i++)
    {
        leds[i] = CHSV(avgPosition, 255, 50);
    }
}