int ledOffsets[LED_CNT] = {};

#include <LEDStrip.h>
#include <EffectTypes.h>
#include <Helpers.h>

const int EFFECT_CNT = 5;

int effectLoopClockOffset = 0;
const unsigned int effectRefreshHz = 60; // Updates every 50mz (20hz);
unsigned long lastRefreshMs = 0;

void zeroOutStrip()
{
  for (int i = 0; i < LED_CNT + LED_OFFSET - 1; i++) {
    cleds[i] = CRGB(0, 0, 0);
  }
 
}

void generateRandom() {
  for (int i = 0; i < LED_CNT; i++) {
    ledOffsets[i] = random(0, LED_CNT);
  }
}

void effectSetup()
{
  ledStripSetup();
  zeroOutStrip();
  generateRandom();
}

void copyLedsWithOffset()
{
  for (int i = 0; i < LED_CNT; i++)
  {
    cleds[i + LED_OFFSET] = leds[i];
  }

}

void copyLedsWithOffsetGamma()
{
  for (int i = 0; i < LED_CNT; i++)
  {
    const int idx = i + LED_OFFSET;
    cleds[idx].r = dim8_video(leds[idx].r);
    cleds[idx].g = dim8_video(leds[idx].g);
    cleds[idx].b = dim8_video(leds[idx].b);
    // cleds[i + LED_OFFSET] = leds[i];
  }

}


// void effectClearLeds()
// {
//     for (int i = 0; i < LED_CNT; i++)
//     {
//         leds[i] = BlackLightFluorescent;
//     }
// }

void recievedStatusEffect(CRGB color, int d)
{
  for (int i = 0; i < LED_CNT; i++)
  {
    leds[i] = color;
  }
  copyLedsWithOffset();
  delay(d);
}

#include <Effects/All.h>

// EFFECT_LOOP_MS
void effectLoop(struct EffectState *effectState)
{

  if (millis() < lastRefreshMs + (1000 / effectRefreshHz))
  {
    return;
  }

  lastRefreshMs = millis();

  switch (effectState->activeEffect)
  {
  case 255: // This is the loading one.
    throbEffectLoop(effectState, 0);
    break;
  case 1:
    hueSpinEffectLoop(effectState, 5000);
    break;
  case 0:
    spinEffectLoop(effectState);
    break;
  case 2:
    strobeEffectLoop(effectState, CRGB(0, 0, 0), CRGB(100, 100, 100), 100);
    break;
  case 3:
    strobeEffectLoop(effectState, CRGB(255, 0, 0), CRGB(0, 0, 255), 100);
    break;
  case 4:
    strobeEffectLoop(effectState, CRGB(0, 0, 0), CRGB(50, 50, 50), 500);
    break;
  default:
    Serial.println("Please set effect count correctly");
    Serial.println(effectState->activeEffect);
  }

  Serial.println('dd');

}
