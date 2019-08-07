#include <EffectTypes.h>

const CRGB RED = CRGB(255, 0, 0);
const CRGB BLUE = CRGB(255, 0, 0);

const unsigned int blinkTs = 20;
const unsigned int steps = 10;
const unsigned int loopMs = 1000;

void colorStrobeEffectLoop(struct EffectState *effectState)
{
  const unsigned int loopPosition = effectState->loopPosition % loopMs;
  const float loopPercent = float(loopPosition) / loopMs;
  const unsigned int step = loopPercent * steps;
  const unsigned int stepBase = (loopMs / steps) * step;

  const unsigned int hueLow = 170;
  const unsigned int hueHigh = 300;
  
  const unsigned int hue = hueLow + (hueHigh - hueLow) * loopPercent;

  for (int i = 0; i < LED_CNT; i++) {
    if (loopPosition < stepBase + blinkTs) {
      leds[i] = CHSV(hue, 220,255);
    } else {
      leds[i] = CRGB(0,0,0);
    }
  
  }
    copyLedsWithOffset();
}