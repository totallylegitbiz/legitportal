#include <Helpers.h>

const int EFFECT_CNT = 5;

uint32_t effectLoopClockOffset = 0;
const uint8_t effectRefreshHz = 100; // Updates 100 times a second.
uint32_t lastRefreshMs = 0;

void effectSetup()
{
  zeroOutStrip();
}

void recievedStatusEffect(CRGB color, int d)
{
  for (uint16_t i = 0; i < LED_CNT; i++)
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
const uint8_t SPARKLE_WHITE = 20;
const uint8_t SPARKLE_RAINBOW = 21;

// Solid colors
const uint8_t SOLID_0_EFFECT = 100;
const uint8_t SOLID_1_EFFECT = 101;
const uint8_t SOLID_2_EFFECT = 102;
const uint8_t SOLID_3_EFFECT = 103;
const uint8_t SOLID_4_EFFECT = 104;
const uint8_t SOLID_5_EFFECT = 105;
const uint8_t SOLID_6_EFFECT = 106;
const uint8_t SOLID_7_EFFECT = 107;
const uint8_t SOLID_8_EFFECT = 108;
const uint8_t SOLID_9_EFFECT = 109;
const uint8_t SOLID_10_EFFECT = 110;

// TODO(jorgelo): Do something like this.
// const uint8_t BIKE_EFFECTS[] = {CANDLE_EFFECT, POLICE_EFFECT, HUE_SPIN_EFFECT};

void effectRenderLoop(uint8_t effectId, struct EffectDataPacket *effectState)
{

  switch (effectId)
  {
  case LOADING_EFFECT: // This is the loading one.
    throbEffectLoop(effectState, 0);
    break;
  case DOT_SPIN_EFFECT:
    // spinEffectLoop(effectState, 2000);
    throbEffectLoop(effectState, 100);
    break;
  case HUE_SPIN_EFFECT:
    hueSpinEffectLoop(effectState, 5000);
    break;
  case STOBE_FLASH_EFFECT:
    policeEffectLoop(effectState);
    // strobeEffectLoop(effectState, CRGB(0, 0, 0), CRGB(100, 100, 100), 100);
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
    hueBarsEffectLoop(effectState, 10000);
    break;
  case SOLID_0_EFFECT:
    solidEffectLoop(effectState, CHSV(0, 255, 255));
    break;
  case SOLID_1_EFFECT:
    solidEffectLoop(effectState, CHSV(25, 255, 255));
    break;
  case SOLID_2_EFFECT:
    solidEffectLoop(effectState, CHSV(50, 255, 255));
    break;
  case SOLID_4_EFFECT:
    solidEffectLoop(effectState, CHSV(75, 255, 255));
    break;
  case SOLID_5_EFFECT:
    solidEffectLoop(effectState, CHSV(100, 255, 255));
    break;
  case SOLID_6_EFFECT:
    solidEffectLoop(effectState, CHSV(125, 255, 255));
    break;
  case SOLID_7_EFFECT:
    solidEffectLoop(effectState, CHSV(150, 255, 255));
    break;
  case SOLID_8_EFFECT:
    solidEffectLoop(effectState, CHSV(175, 255, 255));
    break;
  case SOLID_9_EFFECT:
    solidEffectLoop(effectState, CHSV(200, 255, 255));
    break;
  case SOLID_10_EFFECT:
    solidEffectLoop(effectState, CHSV(225, 255, 255));
    break;
  case SPARKLE_WHITE:
    sparkleEffectLoop(effectState, 0);
    break;
  case SPARKLE_RAINBOW:
    sparkleEffectLoop(effectState, 200);
    break;
  default:
    Serial.println("Please set effect count correctly");
    Serial.println(effectState->activeEffect);
  }
}

void effectLoop(struct EffectDataPacket *effectState)
{

  if (millis() < lastRefreshMs + (1000 / effectRefreshHz))
  {
    return;
  }

  lastRefreshMs = millis();

  effectRenderLoop(effectState->activeEffect, effectState);

  if (overRideUntilTs > 0 && millis() > overRideUntilTs)
  {
    Serial.println("Ending override..");
    overRideUntilTs = 0;
  }

  show_at_max_brightness_for_power();
}