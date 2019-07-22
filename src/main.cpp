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

int pingOffset = 3000;
bool lastOffset = pingOffset;
int lastRecievedOffset = 0;
void loop()
{

  const long time = millis();

  const int offset = time % pingOffset;

  if (offset < lastOffset)
  {
    // Do the thing.
    int pingOffset = random(1 * 1000, 3 * 1000);
    radio.stopListening();
    delay(100);
    blink(GREEN_LED_PIN);

    char newOffset[] = "supyo";
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
  }
  lastOffset = offset;

  if (radio.available())
  {
    char nextOffset[32] = "";
    radio.read(&nextOffset, sizeof(nextOffset));
    Serial.print("GOT: ");
    Serial.println(nextOffset);
    blink(GREEN_LED_PIN);
  }

  for (int i = 0; i < NUM_LEDS; i++)
  {
    if (i < NUM_LEDS * offset / pingOffset)
    {
      leds[i] = CHSV(50, 100, 100);
    }
    else
    {
      leds[i] = CHSV(0, 200, 100);
    }
  }

  FastLED.show();
}