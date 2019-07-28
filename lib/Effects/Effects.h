#include <LEDStrip.h>
#include <EffectTypes.h>
#include <Effects/All.h>

const int EFFECT_CNT = 2;

void zeroOutStrip()
{
    for (int i = 0; i < LED_CNT + LED_OFFSET - 1; i++)
    {
        cleds[i] = CRGB(0, 0, 0);
    }
    FastLED.show();
}

void effectSetup()
{
    ledStripSetup();
    zeroOutStrip();
}

void copyLedsWithOffset()
{
    for (int i = 0; i < LED_CNT; i++)
    {
        cleds[i + LED_OFFSET] = leds[i];
    }
    FastLED.show();
}

// void effectClearLeds()
// {
//     for (int i = 0; i < LED_CNT; i++)
//     {
//         leds[i] = BlackLightFluorescent;
//     }
// }

// EFFECT_LOOP_MS
void effectLoop(struct EffectState *effectState)
{

    switch (effectState->activeEffect)
    {
    case 0:
        strobeEffectLoop(effectState);
        break;
    case 1:
        spinEffectLoop(effectState);
        break;
    }

    copyLedsWithOffset();
}
