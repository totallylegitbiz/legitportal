
#include <LEDStrip.h>
#include <EffectTypes.h>
#include <Effects/All.h>

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

// EFFECT_LOOP_MS
void effectLoop(struct EffectState *effectState)
{
    spinEffectLoop(effectState);

    copyLedsWithOffset();
}