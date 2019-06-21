#include <FastLED.h>
#include <math.h>
#include "tlc/Ringer.ino"
#define LED_PIN 13
#define NUM_LEDS 150
#define POT1_PIN 0
#define BUTTON1_PIN 12
#define DEBUG 1

CRGB leds[NUM_LEDS];

//TotallyLegitController tlc;
Dashspin dashspin;

int offset = 0;

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(9600);
  Serial.print("Starting...");

  pinMode(BUTTON1_PIN, INPUT);

  // Setup dashsin here
  // dashpin.config
  
//  tlc.setup();

//  dashspin.set();

}


void loop() {
  int button1State = digitalRead(BUTTON1_PIN);
  float pot1Percent = (float) analogRead(POT1_PIN) / 1024;
  
  int cycleMs = 1 * 1000;
  int offset = millis() % cycleMs;
  
  float basePercent = (float) offset / cycleMs;
  
  CRGB cleds[NUM_LEDS];

  dashspin.loop(cleds, basePercent);
  
//  tlc.loop(cleds, basePercent);
  
  for (int i = 0; i < NUM_LEDS; i++) {  
    leds[i] = cleds[i];
  }

  FastLED.show();
 
}
