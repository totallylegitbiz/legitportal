#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>
#include <FastLED.h>

// Config
#define RED_LED_PIN 2
#define GREEN_LED_PIN 3
#define BLUE_LED_PIN 4

#define LED_PIN 6
const int NUM_LEDS = 10;

// LED stuff
CRGB leds[NUM_LEDS];

void blink(int pin)
{
  digitalWrite(pin, HIGH);
  delay(50);
  digitalWrite(pin, LOW);
}

RF24 radio(7, 8); // CE, CSN pins

//address through which two modules communicate.
const byte address[6] = "00001";

void setup()
{
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  blink(RED_LED_PIN);
  blink(GREEN_LED_PIN);
  blink(BLUE_LED_PIN);

  Serial.begin(9600);
  radio.begin();
  // radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN);
  radio.setAutoAck(false);
  // radio.setAutoAck(true);
  radio.disableDynamicPayloads();
  radio.setRetries(15, 15);

  radio.openReadingPipe(0, address);
  radio.openWritingPipe(address);

  radio.startListening();

  printf_begin();
  radio.printDetails();

  Serial.println("READY!");
  randomSeed(analogRead(0));
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

  for (int i = 0; i < NUM_LEDS; i++)
  {
    // if (i < (NUM_LEDS * effectLoopOffsetPercent))
    // {
    //   leds[i] = CHSV(50, 100, 100);
    // }
    // else
    // {

    // }

    leds[i] = CHSV(effectLoopOffsetPercent * 255, 255, 100);
  }
  FastLED.show();
}