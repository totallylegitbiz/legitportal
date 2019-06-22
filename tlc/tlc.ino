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
Dashspin dashspin1(2, .03);

int offset = 0;
float velocity = 0;

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(9600);
  Serial.print("Starting...");

  pinMode(BUTTON1_PIN, INPUT); 
}

void loop() {

  int cycleSteps = NUM_LEDS * 1024;
  float MAX_VELOCITY = NUM_LEDS / 2;
  
  int button1State = digitalRead(BUTTON1_PIN);
  float pot1Percent = 1-(float) analogRead(POT1_PIN) / 1024;

  CRGB cleds[NUM_LEDS];
  
  velocity = (pot1Percent-.5) * MAX_VELOCITY;

  if (velocity > MAX_VELOCITY) {
    velocity = MAX_VELOCITY;
  }
  
  int nextOffset = velocity + offset;

  if (nextOffset > 0) {
     offset = nextOffset % cycleSteps;
  } else {
     offset = cycleSteps - (nextOffset % cycleSteps);
  }
  
  float basePercent = (float) offset / cycleSteps;
 
  dashspin1.loop(cleds, basePercent);
  
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
