#include <FastLED.h>
#include <Config.h>

#ifndef INC_EFFECT_TYPES
#define INC_EFFECT_TYPES

// This is our local copy of leds.
CRGB leds[LED_CNT];

typedef struct EffectState
{
    unsigned int loopPosition = 0;
    int activeEffect = 0;
};

#endif
