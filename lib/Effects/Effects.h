#include <Helpers.h>

uint32_t effectLoopClockOffset = 0;
const uint8_t effectRefreshHz = 60; // Updates 60 times a second.
uint32_t lastRefreshMs = 0;

void effectSetup()
{
  zeroOutStrip();
}

#include <Effects/All.h>

int cnt = 0;
const uint8_t LOADING_EFFECT = 255;
const uint8_t DOT_SPIN_EFFECT = 0;
const uint8_t HUE_SPIN_EFFECT = 1;
const uint8_t STOBE_FLASH_EFFECT = 2;
// const uint8_t BLUE_RED_FLASH_EFFECT = 3;
const uint8_t STOBE_SLOW_EFFECT = 4;
const uint8_t HUE_SPARKLE_LIGHT_EFFECT = 5;
const uint8_t HUE_SPARKLE_DARK_EFFECT = 6;
const uint8_t CANDLE_EFFECT = 7;
const uint8_t POLICE_EFFECT = 8;
const uint8_t PURPLE_BLUES_EFFECT = 9;
const uint8_t HUE_BARS_EFFECT = 10;
const uint8_t SPARKLE_WHITE = 20;
const uint8_t SPARKLE_RAINBOW = 21;
const uint8_t SLOW_HUE = 22;
const uint8_t DASH_SPIN_SLOW = 23;
const uint8_t DASH_SPIN_FAST = 24;
const uint8_t DASH_SPIN_SLOW_HUE = 25;
const uint8_t DASH_SPIN_FAST_HUE = 26;
const uint8_t STOBE_FLASH_HUE_EFFECT = 27;
const uint8_t STOBE_SLOW_HUE_EFFECT = 28;

const uint8_t HUE_BARS_1_EFFECT = 40;
const uint8_t HUE_BARS_2_EFFECT = 41;
const uint8_t HUE_BARS_3_EFFECT = 42;
const uint8_t HUE_BARS_4_EFFECT = 43;
const uint8_t HUE_BARS_5_EFFECT = 44;
const uint8_t HUE_BARS_6_EFFECT = 45;
const uint8_t HUE_BARS_7_EFFECT = 46;
const uint8_t HUE_BARS_8_EFFECT = 47;

// Group Effects
const uint8_t COLOR_GROUP = 50;

// Throbs
const uint8_t THROB_01_EFFECT = 60;
const uint8_t THROB_02_EFFECT = 61;
const uint8_t THROB_03_EFFECT = 62;

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
const uint8_t EFFECTS[] = {
    SLOW_HUE, // Leave this first.
    STOBE_FLASH_HUE_EFFECT,
    STOBE_SLOW_EFFECT,
    STOBE_FLASH_EFFECT,
    STOBE_FLASH_HUE_EFFECT,
    STOBE_SLOW_HUE_EFFECT,
    HUE_BARS_1_EFFECT,
    HUE_BARS_2_EFFECT,
    HUE_BARS_3_EFFECT,
    HUE_BARS_4_EFFECT,
    HUE_BARS_5_EFFECT,
    HUE_BARS_6_EFFECT,
    HUE_BARS_7_EFFECT,
    HUE_BARS_8_EFFECT,
    CANDLE_EFFECT,
    COLOR_GROUP,
    DASH_SPIN_FAST,
    DASH_SPIN_FAST_HUE,
    DASH_SPIN_SLOW,
    DASH_SPIN_SLOW_HUE,
    HUE_SPARKLE_DARK_EFFECT,
    HUE_SPARKLE_LIGHT_EFFECT,
    HUE_SPIN_EFFECT,
    POLICE_EFFECT,
    PURPLE_BLUES_EFFECT,
    SPARKLE_RAINBOW,
    SPARKLE_WHITE,
    STOBE_FLASH_EFFECT,
    STOBE_SLOW_EFFECT,
    THROB_01_EFFECT,
    THROB_02_EFFECT,
    THROB_03_EFFECT,
    SOLID_0_EFFECT,
    SOLID_1_EFFECT,
    SOLID_2_EFFECT,
    SOLID_3_EFFECT,
    SOLID_4_EFFECT,
    SOLID_5_EFFECT,
    SOLID_6_EFFECT,
    SOLID_7_EFFECT,
    SOLID_8_EFFECT,
    SOLID_9_EFFECT,
    SOLID_10_EFFECT,
};

void effectRenderLoop(uint8_t effectIdx, struct EffectDataPacket *effectState)
{
  uint8_t effectId;

  if (effectIdx == 255)
  {
    effectId = 255;
  }
  else
  {
    effectId = EFFECTS[effectIdx];
  }

  const uint8_t hue = effectState->transmitterId % 255;

  const uint8_t flashHue = notRandom(0, 255, effectState->loopPosition);

  switch (effectId)
  {
  case LOADING_EFFECT: // This is the loading one.
    throbEffectLoop(effectState, 0);
    break;
  case THROB_01_EFFECT: // This is the loading one.
    throbEffectLoop(effectState, hue, 500);
    break;
  case THROB_02_EFFECT: // This is the loading one.
    throbEffectLoop(effectState, hue, 2500);
    break;
  case THROB_03_EFFECT: // This is the loading one.
    throbEffectLoop(effectState, hue, 5000);
    break;
  case DASH_SPIN_SLOW: // This is the loading one.
    dashSpinEffectLoop(effectState, 1000);
    break;
  case DASH_SPIN_FAST: // This is the loading one.
    dashSpinEffectLoop(effectState, 200);
    break;
  case DASH_SPIN_SLOW_HUE: // This is the loading one.
    dashSpinEffectLoop(effectState, 10000, true);
    break;
  case DASH_SPIN_FAST_HUE: // This is the loading one.
    dashSpinEffectLoop(effectState, 2000, true);
    break;
  case COLOR_GROUP: // This is the loading one.
    groupSolidColorEffectLoop(effectState, 50);
    break;
  case HUE_SPIN_EFFECT:
    hueSpinEffectLoop(effectState, 5000);
    break;
  case SLOW_HUE:
    hueCycleEffectLoop(effectState, config.EFFECT_LOOP_MS, 169, 255);
    break;
  case STOBE_FLASH_EFFECT:
    strobeEffectLoop(effectState, CRGB(0, 0, 0), CHSV(flashHue, 255, 255), 500);
    break;
  case STOBE_SLOW_EFFECT:
    strobeEffectLoop(effectState, CRGB(0, 0, 0), CRGB(50, 50, 50), 500);
    break;
  case STOBE_FLASH_HUE_EFFECT:
    strobeEffectLoop(effectState, CRGB(0, 0, 0), CHSV(flashHue, 255, 255), 500);
    break;
  case STOBE_SLOW_HUE_EFFECT:
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
  case HUE_BARS_1_EFFECT:
    hueBarsEffectLoop(effectState, 1500, 30000, 10);
    break;
  case HUE_BARS_2_EFFECT:
    hueBarsEffectLoop(effectState, 2000, 5000, 5);
    break;
  case HUE_BARS_3_EFFECT:
    hueBarsEffectLoop(effectState, 3000, 500, 10);
    break;
  case HUE_BARS_4_EFFECT:
    hueBarsEffectLoop(effectState, 3000, 10, 10);
    break;
  case HUE_BARS_5_EFFECT:
    hueBarsEffectLoop(effectState, 30000, 10, 40);
    break;
  case HUE_BARS_6_EFFECT:
    hueBarsEffectLoop(effectState, 2000, 10, 10);
    break;
  case HUE_BARS_7_EFFECT:
    hueBarsEffectLoop(effectState, 3000, 300, 30);
    break;
  case HUE_BARS_8_EFFECT:
    hueBarsEffectLoop(effectState, 200, 100, 10);
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
  case SOLID_3_EFFECT:
    solidEffectLoop(effectState, CHSV(75, 255, 255));
    break;
  case SOLID_4_EFFECT:
    solidEffectLoop(effectState, CHSV(100, 255, 255));
    break;
  case SOLID_5_EFFECT:
    solidEffectLoop(effectState, CHSV(125, 255, 255));
    break;
  case SOLID_6_EFFECT:
    solidEffectLoop(effectState, CHSV(150, 255, 255));
    break;
  case SOLID_7_EFFECT:
    solidEffectLoop(effectState, CHSV(175, 255, 255));
    break;
  case SOLID_8_EFFECT:
    solidEffectLoop(effectState, CHSV(200, 255, 255));
    break;
  case SOLID_9_EFFECT:
    solidEffectLoop(effectState, CHSV(225, 255, 255));
    break;
  case SOLID_10_EFFECT:
    solidEffectLoop(effectState, CHSV(250, 255, 255));
    break;
  case SPARKLE_WHITE:
    sparkleEffectLoop(effectState, 0);
    break;
  case SPARKLE_RAINBOW:
    sparkleEffectLoop(effectState, 255);
    break;
  default:
    SERIAL_PRINT("Please set effect count correctly, received: effectIdx = ");
    SERIAL_PRINTLN(effectIdx);
    throbEffectLoop(effectState, 200);
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
    SERIAL_PRINTLN("Ending override..");
    overRideUntilTs = 0;
    effectState->effectModifier = 0;
  }

  show_at_max_brightness_for_power();
}