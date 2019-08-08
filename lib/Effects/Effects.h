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

const uint8_t LOADING_EFFECT = 255;
const uint8_t DOT_SPIN_EFFECT = 0; 
const uint8_t HUE_SPIN_EFFECT = 1;
const uint8_t STOBE_FLASH_EFFECT = 2;
const uint8_t BLUE_RED_FLASH_EFFECT = 3;
const uint8_t STOBE_SLOW_EFFECT = 4;
const uint8_t HUE_SPARKLE_LIGHT_EFFECT = 5;
const uint8_t HUE_SPARKLE_DARK_EFFECT = 6;
const uint8_t CANDLE_EFFECT = 7;
const uint8_t POLICE_EFFECT = 8;
const uint8_t PURPLE_BLUES_EFFECT = 9;
const uint8_t HUE_BARS_EFFECT = 10;

void effectRenderLoop(uint8_t effectId, struct EffectState *effectState)
{

#ifdef EFFECT_OVERIDE
  const uint8_t selectedEffectId = EFFECT_OVERIDE;
#else
  const uint8_t selectedEffectId = effectId;
#endif

  switch (selectedEffectId)
  {
  case LOADING_EFFECT: // This is the loading one.
    throbEffectLoop(effectState, 0);
    break;
  case DOT_SPIN_EFFECT:
    spinEffectLoop(effectState);
    break;
  case HUE_SPIN_EFFECT:
    hueSpinEffectLoop(effectState, 5000);
    break;
  case STOBE_FLASH_EFFECT:
    strobeEffectLoop(effectState, CRGB(0, 0, 0), CRGB(100, 100, 100), 100);
    break;
  case BLUE_RED_FLASH_EFFECT:
    strobeEffectLoop(effectState, CRGB(255, 0, 0), CRGB(0, 0, 255), 100);
    break;
  case STOBE_SLOW_EFFECT:
    strobeEffectLoop(effectState, CRGB(0, 0, 0), CRGB(50, 50, 50), 500);
    break;
  case HUE_SPARKLE_LIGHT_EFFECT:
    fadeSparkleEffectLoop(effectState, false);
    break;
  case HUE_SPARKLE_DARK_EFFECT:
    fadeSparkleEffectLoop(effectState, true);
    break;
  case CANDLE_EFFECT:
    candleEffectLoop(effectState, 1); // No steps
    break;
  case POLICE_EFFECT:
    policeEffectLoop(effectState);
    break;
  case PURPLE_BLUES_EFFECT:
    hueSparkleEffectLoop(effectState);
    break;
  case HUE_BARS_EFFECT:
    hueBarsEffectLoop(effectState,10000);
    break;
  default:
    Serial.println("Please set effect count correctly");
    Serial.println(effectState->activeEffect);
  }
}

void effectLoop(struct EffectState *effectState)
{

  if (millis() < lastRefreshMs + (1000 / effectRefreshHz))
  {
    return;
  }

  lastRefreshMs = millis();

  effectRenderLoop(effectState->activeEffect,effectState);
}