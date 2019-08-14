#ifndef CONFIG_H
#define CONFIG_H

#ifndef ELED_CNT
#error SET ELED_CNT PLEASE
#endif

const uint8_t MAX_LEDS = 200; // This is the max number of LEDs for any target.

const uint8_t BIKE = 0;
const uint8_t CHILL_DOME = 1;
const uint8_t DEEP_PLAYA = 2;
const uint8_t CAMP = 3;
const uint8_t PULSE_REMOTE = 4;
const uint8_t PORTAL = 5;

const uint8_t LED_OFFSET = 0;
const uint16_t LED_CNT = ELED_CNT;
const uint8_t LED_PIN = 8;

CRGB cleds[LED_CNT];
CRGB leds[LED_CNT]; // This is our local copy of leds.

// This is the base config.
typedef struct Config
{

  uint8_t DEVICE_TYPE = BIKE;

  bool DIAGNOSTIC_MODE = false;

  // Effect Loop config
  const uint16_t EFFECT_LOOP_MS = 60 * 1000;
  const uint8_t LED_BRIGHTNESS = 20; // 0-255 for overall brightness.

  // RGB Status LED
  const int RED_LED_PIN = A0;
  const int GREEN_LED_PIN = A1;
  const int BLUE_LED_PIN = A2;

  // Dip Config
  const int DIP_PIN_0 = 2;
  const int DIP_PIN_1 = 3;
  const int DIP_PIN_2 = 4;
  const int DIP_PIN_3 = 5;

  // Sensor Pin
  const int SENSOR_PIN = A7;

  // RADIO
  const int RADIO_CE_PIN = 9;
  const int RADIO_CSN_PIN = 10;

  // EFFECT BUTTON
  const int EFFECT_BUTTON_PIN = 6;

  int TRANSMITTER_ID = random(1, 65535);
};

uint8_t getDipValue(Config config)
{
  // DIP
  pinMode(config.DIP_PIN_0, INPUT_PULLUP);
  pinMode(config.DIP_PIN_1, INPUT_PULLUP);
  pinMode(config.DIP_PIN_2, INPUT_PULLUP);
  pinMode(config.DIP_PIN_3, INPUT_PULLUP);

  const bool d0 = digitalRead(config.DIP_PIN_0);
  const bool d1 = digitalRead(config.DIP_PIN_1);
  const bool d2 = digitalRead(config.DIP_PIN_2);
  const bool d3 = digitalRead(config.DIP_PIN_3);

  // Very simple bit math
  return !d0 + (!d1 * 2) + (!d2 * 4) + (!d3 * 8);
}

Config getConfig()
{
  Config outConfig;

  outConfig.DEVICE_TYPE = getDipValue(outConfig);

  switch (outConfig.DEVICE_TYPE)
  {
  case BIKE:
    // Bike specific
    break;
  }

  return outConfig;
}

#ifndef DEFAULT_EFFECT
#define DEFAULT_EFFECT 0;
#endif

typedef struct EffectDataPacket
{
  uint8_t loopPosition = 0;
  uint8_t activeEffect = DEFAULT_EFFECT;
  int16_t sourceTransmitterId = 0; // If the sourceTransmitterId !== transmitterId it's a relay.
  int16_t transmitterId = 0;
  uint32_t age = 0;
};

#endif
