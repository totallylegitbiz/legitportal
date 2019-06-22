#define NUM_LEDS 150

#include <FastLED.h>
#include <math.h>
#include "dashspin.h"
#include "gamma.h";

#define LED_PIN 13

#define POT1_PIN 0
#define BUTTON1_PIN 12
#define DEBUG 1

#define GAMMA 1

CRGB leds[NUM_LEDS];

//TotallyLegitController tlc;
Dashspin dashspin(2, .03);

int offset = 0;
int velocity = 0;

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(9600);
  Serial.print("Starting...");

  pinMode(BUTTON1_PIN, INPUT); 
}

void loop() {

  int MAX_VELOCITY = 250;
  int button1State = digitalRead(BUTTON1_PIN);
  float pot1Percent = 1-(float) analogRead(POT1_PIN) / 1024;

  CRGB cleds[NUM_LEDS];
  
  velocity = (pot1Percent-.5) * MAX_VELOCITY;

  if (velocity > MAX_VELOCITY) {
    velocity = MAX_VELOCITY;
  }
  
  int cycleMs = 5 * 1000;
  
  int nextOffset = velocity + offset;

  if (nextOffset > 0) {
     offset = nextOffset % cycleMs;
  } else {
     offset = cycleMs - (nextOffset % cycleMs);
  }

//  offset = (1-pot1Percent) * cycleMs;
  
  float basePercent = (float) offset / cycleMs;
 
  dashspin.loop(cleds, basePercent);
  
  for (int i = 0; i < NUM_LEDS; i++) {  
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
