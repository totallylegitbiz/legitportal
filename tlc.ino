#define NUM_LEDS 150

#include <FastLED.h>
#include <math.h>

#include "controller.h"
#include "dashspin.h"
#include "gamma.h"

#define LED_PIN 13

#define POT0_PIN 0
#define POT1_PIN 1

#define BUTTON0_PIN 12
#define BUTTON1_PIN 13

#define DEBUG 1

#define GAMMA 0

CRGB leds[NUM_LEDS];

unsigned int cycleSteps = NUM_LEDS * 100;
float MAX_VELOCITY = NUM_LEDS;

#define EFFECT_FIELDS 2
#define EFFECT_TYPE_DASHPIN 0

int effectConfig[][EFFECT_FIELDS] = {
    {EFFECT_TYPE_DASHPIN, 1}};

TotallyLegitController tlc;
//Dashspin dashspin1(1, .1);

int offset = 0;
int mode = 0;
float velocity = 0;

// Current state
float percent[2] = {.5, .5};
int lastButtonState[2] = {0, 0};

struct InputState
{
  float percent[2];
  int fields[EFFECT_FIELDS];
  int buttonMode[2];
  int buttonState[2];
};

void setup()
{
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(9600);
  Serial.print("Starting...");

  pinMode(BUTTON0_PIN, INPUT);
  pinMode(BUTTON1_PIN, INPUT);
}

void loop()
{

  int VELOCITY_POT = 0;
  int EFFECT_POT = 1;

  CRGB cleds[2][NUM_LEDS];

  int buttonState[2] = {
      digitalRead(BUTTON0_PIN),
      digitalRead(BUTTON1_PIN)};

  float potPercent[2] = {
      1 - (float)analogRead(POT0_PIN) / 1024,
      1 - (float)analogRead(POT1_PIN) / 1024};

  int buttonMode[2] = {0, 0};

  for (int i = 0; i < 2; i++)
  {
    if (buttonState[i] != lastButtonState[i])
    {
      lastButtonState[i] = buttonState[i];
      buttonMode[i] = buttonState[i];
    };
  };

  float potPercent[2] = {
      1 - (float)analogRead(POT0_PIN) / 1024,
      1 - (float)analogRead(POT1_PIN) / 1024};

  float velocity = (potPercent[VELOCITY_POT] - .5) * 500;
  int nextOffset = velocity + offset;

  int effectCnt = sizeof(effectConfig);
  //  float effectPosition = (effectCnt - 1) * potPercent[EFFECT_POT];
  int effectPosition = effectCnt * potPercent[EFFECT_POT];

  // TODO(jorgelo): This could probably be done in one line.
  if (nextOffset > 0)
  {
    offset = nextOffset % cycleSteps;
  }
  else
  {
    offset = cycleSteps - (nextOffset % cycleSteps);
  }

  float basePercent = (float)offset / cycleSteps;

  //  dashspin1.loop(cleds, basePercent,secondPercent);

  for (int i = 0; i < NUM_LEDS; i++)
  {
#if GAMMA
    leds[i].r = pgm_read_byte(&gamma8[cleds[i].r]);
    leds[i].g = pgm_read_byte(&gamma8[cleds[i].g]);
    leds[i].b = pgm_read_byte(&gamma8[cleds[i].b]);
#else
    leds[i] = cleds[i];
#endif
  }

  FastLED.show();
}

//struct DFData {

/*
struct DFData {
  char* name;
  byte dirCode;
  int val1;
  int val2;
  int val3;
  void print() {
    Serial.print(F("{\""));
    Serial.print(name);
    Serial.print(F("\", 0x"));
    if (dirCode < 16) {
      Serial.write('0');
    }
    Serial.print(dirCode, HEX);
    Serial.print(F(", "));
    Serial.print(val1);
    Serial.print(F(", "));
    Serial.print(val2);
    Serial.print(F(", "));
    Serial.print(val3);
    Serial.println(F("},"));
  }
} dfData[] = {
  {"NAME1", 0x07, 10, 1, 58},
  {"NAME2", 0x08, 7, 3, 20},
  {"NAME3", 0x09, 12, 6, 12},
};

void setup() {
  Serial.begin(250000);
  for (byte i = 0; i < sizeof(dfData) / sizeof(dfData[0]); i++) {
    dfData[i].print();
  }
}
void loop() {}
*/
