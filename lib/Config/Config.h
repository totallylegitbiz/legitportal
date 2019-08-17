#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG 1

#include <Random.h>

#ifndef ELED_CNT
#error SET ELED_CNT PLEASE
#else
const uint16_t LED_CNT = ELED_CNT;
#endif

const uint8_t MAX_LEDS = 200; // This is the max number of LEDs for any target.

const uint8_t BIKE = 0;
const uint8_t CHILL_DOME = 1;
const uint8_t DEEP_PLAYA = 2;
const uint8_t CAMP = 3;
const uint8_t PULSE_REMOTE = 4;
const uint8_t PORTAL = 5;

const uint8_t LED_OFFSET = 0;

const uint8_t LED_PIN = 8;

// CRGB cleds[LED_CNT];
CRGB leds[LED_CNT]; // This is our local copy of leds.

const int UNUSED_PIN = A3;

enum class DeviceRole
{
  BIKE = 0,
  CAMP = 1,
  // CHILL_DOME = 2,
  // DEEP_PLAYA = 3,
  // PULSE_REMOTE = 5,
  // PORTAL = 6,
};

// This is the base config.
typedef struct Config
{
  // Effect Loop config
  const uint32_t EFFECT_LOOP_MS = 300000; // 5 minute loop

  // LED
  // const uint8_t LED_BRIGHTNESS = 255; // 0-255 for overall brightness.
  // Parameters for  FastLED.setMaxPowerInVoltsAndMilliamps
  const uint8_t MAX_VOLTS = 5;
  uint32_t MAX_MAMP = 50; // TODO(jorgelo): Make this a build flag?

  // RGB Status LED
  const int RED_LED_PIN = A0;
  const int GREEN_LED_PIN = A1;
  const int BLUE_LED_PIN = A2;

  // Dip Config
  // const int DIP_PIN_0 = 2;
  // const int DIP_PIN_1 = 3;
  // const int DIP_PIN_2 = 4;
  // const int DIP_PIN_3 = 5;

  // Sensor Pin
  const int SENSOR_PIN = A3;

  // RADIO
  const int RADIO_CE_PIN = 9;
  const int RADIO_CSN_PIN = 10;

  // EFFECT BUTTON
  const int EFFECT_BUTTON_PIN = 6;

  uint16_t TRANSMITTER_ID;
  DeviceRole ROLE = DeviceRole::BIKE;
};

// uint8_t getDipValue(Config config)
// {
//   // DIP
//   pinMode(config.DIP_PIN_0, INPUT_PULLUP);
//   pinMode(config.DIP_PIN_1, INPUT_PULLUP);
//   pinMode(config.DIP_PIN_2, INPUT_PULLUP);
//   pinMode(config.DIP_PIN_3, INPUT_PULLUP);

//   const bool d0 = digitalRead(config.DIP_PIN_0);
//   const bool d1 = digitalRead(config.DIP_PIN_1);
//   const bool d2 = digitalRead(config.DIP_PIN_2);
//   const bool d3 = digitalRead(config.DIP_PIN_3);

//   // Very simple bit math
//   return !d0 + (!d1 * 2) + (!d2 * 4) + (!d3 * 8);
// }

DeviceRole getDeviceRole()
{

#ifndef EDEVICE_ROLE
#error Please set DEVICE_ROLE
#else
  switch ((DeviceRole)EDEVICE_ROLE)
  {
  case DeviceRole::BIKE:
    return DeviceRole::BIKE;
  case DeviceRole::CAMP:
    return DeviceRole::CAMP;
  }
#endif
}

Config getConfig()
{
  Config outConfig;

  reseedRandom();

  outConfig.ROLE = getDeviceRole();
  outConfig.TRANSMITTER_ID = random(1, 65535);

  switch (outConfig.ROLE)
  {
  case DeviceRole::BIKE:
    // Bike specific
    break;
  case DeviceRole::CAMP:
    // Camp specific
    outConfig.MAX_MAMP = 1000;
    break;
  }

  return outConfig;
}

#if !(EDEFAULT_EFFECT + 0)
#define EDEFAULT_EFFECT 255
#endif

typedef struct EffectDataPacket
{
  uint32_t loopPosition = 0;
  uint8_t activeEffect = EDEFAULT_EFFECT;
  uint16_t sourceTransmitterId; // If the sourceTransmitterId !== transmitterId it's a relay.
  uint16_t transmitterId;
  DeviceRole role;
  uint32_t age = 0;
};

#ifdef DEBUG
#define SERIAL_PRINT(x) SERIAL_PRINT(x)
#define SERIAL_PRINTLN(x) SERIAL_PRINTln(x)
#define SERIAL_BEGIN(x) Serial.begin(x)
#else
#define SERIAL_PRINT(x)
#define SERIAL_PRINTLN(x)
#define SERIAL_BEGIN(x)
#endif

#endif
