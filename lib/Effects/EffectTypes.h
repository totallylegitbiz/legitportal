#include <FastLED.h>
#include <Config.h>

#ifndef EFFECT_TYPES_H
#define EFFECT_TYPES_H

// This is our local copy of leds.
CRGB leds[LED_CNT];

typedef struct EffectState
{
    unsigned int loopPosition = 0;
    unsigned int activeEffect = 0;
    unsigned int transmitterId = 0;
    int16_t sourceTransmitterId = 0;
    unsigned long age = 0;
};

#endif
