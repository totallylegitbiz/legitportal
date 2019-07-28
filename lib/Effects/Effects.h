#include <LEDStrip.h>
#include <EffectTypes.h>
#include <Effects/All.h>

const int EFFECT_CNT = 3;

int effectLoopClockOffset = 0;

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

void recievedStatusEffect()
{
    for (int i = 0; i < LED_CNT; i++)
    {
        leds[i] = BlackLightFluorescent;
    }
    copyLedsWithOffset();
    delay(50);
}

// EFFECT_LOOP_MS
void effectLoop(struct EffectState *effectState)
{

    switch (effectState->activeEffect)
    {
    case 255: // This is the loading one.
        throbEffectLoop(effectState, 0);
        break;
    case 0:
        spinEffectLoop(effectState);
        break;
    case 1:
        strobeEffectLoop(effectState, CRGB(0, 0, 0), CRGB(100, 100, 100), 100);
        break;
    case 2:
        strobeEffectLoop(effectState, CRGB(255, 0, 0), CRGB(0, 0, 255), 100);
        break;
    default:
        Serial.println("Please set effect count correctly");
    }

    copyLedsWithOffset();
}
