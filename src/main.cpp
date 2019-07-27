#include <SPI.h>
#include <Config.h>

//address through which two modules communicate.

void setup()
{

  Serial.begin(9600);
}

int pingIntervalMs = 3000;
bool pingLastOffset = pingIntervalMs;

int effectLoopIntervalMs = 5000;
bool effectLoopLastOffset = effectLoopIntervalMs;

int lastRecievedOffset = 0;
int effectLoopClockOffset = 0;

void loop()
{

  // This is the ping loop
  const int pingOffset = millis() % pingIntervalMs;

  if (pingOffset < pingLastOffset)
  {
    // This happenes every ping interval;
    pingIntervalMs = random(1 * 1000, 3 * 1000); // Let's randomize it.

    radio.stopListening();
    blink(GREEN_LED_PIN);

    int newOffset = (millis() + effectLoopClockOffset) % effectLoopIntervalMs;

    bool ok = radio.write(&newOffset, sizeof(newOffset));

    if (ok)
    {
      Serial.print("sent: ");
      Serial.println(newOffset);
      blink(BLUE_LED_PIN);
    }
    else
    {
      Serial.println("failed...");
      blink(RED_LED_PIN);
    }

    radio.startListening();

    blink(GREEN_LED_PIN);
  }

  if (radio.available())
  {
    int nextEffectOffset;
    radio.read(&nextEffectOffset, sizeof(nextEffectOffset));
    Serial.print("nextEffectOffset: ");
    Serial.println(nextEffectOffset);

    int nextEffectLoopClockOffset = nextEffectOffset - (millis() % effectLoopIntervalMs);

    //  TODO(jorgelo): Some logic here incase the drift is too great.
    effectLoopClockOffset = nextEffectLoopClockOffset;

    Serial.print("effectLoopClockOffset: ");
    Serial.println(effectLoopClockOffset);

    blink(GREEN_LED_PIN);
  }

  // Effect stuff

  const int effectLoopOffset = (millis() + effectLoopClockOffset) % effectLoopIntervalMs;
  const float effectLoopOffsetPercent = float(effectLoopOffset) / effectLoopIntervalMs;

  for (int i = 0; i < LED_CNT; i++)
  {
    // if (i < (LED_CNT * effectLoopOffsetPercent))
    // {
    //   leds[i] = CHSV(50, 100, 100);
    // }
    // else
    // {

    // }

    leds[i] = CHSV(effectLoopOffsetPercent * 255, 255, 70);
  }
  FastLED.show();
}