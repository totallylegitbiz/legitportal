#include <SPI.h>
#include <Config.h>
#include <Effects.h>

void setup()
{

  Serial.begin(9600);
  effectSetup();
}

int effectLoopIntervalMs = 5000;
bool effectLoopLastOffset = effectLoopIntervalMs;

int lastRecievedOffset = 0;
int effectLoopClockOffset = 0;

void loop()
{

  // EFFECT_LOOP_MS

  const int loopPosition = millis() % EFFECT_LOOP_MS;
  effectLoop(loopPosition);

  // const int effectLoopOffset = (millis() + effectLoopClockOffset) % effectLoopIntervalMs;
  // const float effectLoopOffsetPercent = float(effectLoopOffset) / effectLoopIntervalMs;

  // for (int i = 0; i < LED_CNT; i++)
  // {
  //   // if (i < (LED_CNT * effectLoopOffsetPercent))
  //   // {
  //   //   leds[i] = CHSV(50, 100, 100);
  //   // }
  //   // else
  //   // {

  //   // }

  //   leds[i] = CHSV(effectLoopOffsetPercent * 255, 255, 70);
  // }
  // FastLED.show();
}