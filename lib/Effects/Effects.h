// #include <FastLED.h>
#include <LEDStrip.h>

// This is what is actually
CRGB leds[LED_CNT];

void zeroOutStrip()
{
    for (int i = 0; i < LED_CNT + LED_OFFSET; i++)
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
void effectLoop(int loopPosition)
{
    for (int i = 0; i < LED_CNT; i++)
    {
        leds[i] = CRGB(0, 255, 0);
    }

    copyLedsWithOffset();
    // FastLED.show();
}

// void effectExample(*CRGB cleds)
// {
// }