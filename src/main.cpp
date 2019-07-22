#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>
#include <FastLED.h>

#define RECEIVER 1
#define TRANSMITTER 2

// Config

#ifndef TYPE
#error "Set TYPE!"
#endif

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

// bool radioNumber = 1;

//create an RF24 object
RF24 radio(7, 8); // CE, CSN

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

#if TYPE == RECEIVER

  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("RECEIVER");
  radio.openReadingPipe(0, address);

  // radio.disableDynamicPayloads();
  // radio.setAutoAck(true);
  // radio.setRetries(15, 15);

  //Set module as receiver
  radio.startListening();
#endif

#if TYPE == TRANSMITTER
  Serial.println("TRANSMITTER");
  digitalWrite(LED_BUILTIN, HIGH);
  // radio.disableDynamicPayloads();
  // radio.setAutoAck(false);
  // radio.setRetries(15, 15);

  //set the address
  radio.openWritingPipe(address);

  // radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  // radio.printDetails();
#endif

  printf_begin();
  radio.printDetails();

  Serial.println("READY!");
}

void loop()
{
#if TYPE == RECEIVER
  //Read the data if available in buffer
  if (radio.available())
  {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
    blink(GREEN_LED_PIN);
  }

#endif

#if TYPE == TRANSMITTER
  //Send message to receiver
  const char text[] = "Hello World";
  bool ok = radio.write(&text, sizeof(text));

  if (ok)
  {
    Serial.println("sent..");
    blink(BLUE_LED_PIN);
  }
  else
  {
    Serial.println("failed...");

    blink(RED_LED_PIN);
  }

  delay(250);
#endif

  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV(50, 100, 100);
  }

  FastLED.show();
}