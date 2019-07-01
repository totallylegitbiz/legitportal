#define NUM_LEDS 150

#include <FastLED.h>
#include <math.h>

#include "util.h"
#include "controller.h"
#include "dashspin.h"
#include "gamma.h"

#define LED_PIN 13

#define POT0_PIN 0
#define POT1_PIN 1

#define BUTTON0_PIN 12
#define BUTTON1_PIN 13

#define DEBUG 1

#define GAMMA 1

CRGB leds[NUM_LEDS];

unsigned int cycleSteps = NUM_LEDS * 100;
float MAX_VELOCITY = NUM_LEDS;

#define EFFECT_FIELDS 2
#define EFFECT_TYPE_DASHPIN 0

int effectConfig[][EFFECT_FIELDS] = {
    {EFFECT_TYPE_DASHPIN, 1}};

TotallyLegitController tlc;
Dashspin dashspin1(1, .1);

int offset = 0;
int mode = 0;
float velocity = 0;

// Current state
float percent[2] = {.5, .5};
int lastButtonState[2] = {0, 0};

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

  float velocity = (potPercent[VELOCITY_POT] - .5) * 500;
  int nextOffset = velocity + offset;

  Serial.print("velocity");
  Serial.print(velocity);
  Serial.print("\n");
  
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

  InputState inputState = {
    {potPercent[0], potPercent[1]},
//    buttonMode,
//    buttonState
  };
   
  

  dashspin1.loop(cleds[0], &inputState);

  for (int i = 0; i < NUM_LEDS; i++)
  {
    #if GAMMA
      leds[i].r = pgm_read_byte(&gamma8[cleds[0][i].r]);
      leds[i].g = pgm_read_byte(&gamma8[cleds[0][i].g]);
      leds[i].b = pgm_read_byte(&gamma8[cleds[0][i].b]);
      Serial.print(leds[i].r);
      Serial.print("\n");
    #else
      leds[i] = cleds[i];
    #endif
  }

  FastLED.show();
}
