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
    SLOW_HUE,
    HUE_BARS_EFFECT,
    PURPLE_BLUES_EFFECT,
    POLICE_EFFECT,
    STOBE_SLOW_EFFECT,
    HUE_SPIN_EFFECT,
    STOBE_FLASH_EFFECT,
    STOBE_SLOW_EFFECT,
    HUE_SPARKLE_LIGHT_EFFECT,
    HUE_SPARKLE_DARK_EFFECT,
    SPARKLE_RAINBOW,
    SPARKLE_WHITE,
    HUE_BARS_EFFECT,
    POLICE_EFFECT,
    CANDLE_EFFECT,
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
    PURPLE_BLUES_EFFECT,
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
  case SLOW_HUE:
    hueCycleEffectLoop(effectState, config.EFFECT_LOOP_MS, 169, 255);
    break;
  case STOBE_FLASH_EFFECT:
    strobeEffectLoop(effectState, CRGB(0, 0, 0), CRGB(100, 100, 100), 100);
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
    sparkleEffectLoop(effectState, 200);
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