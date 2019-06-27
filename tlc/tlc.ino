#define NUM_LEDS 150

#include <FastLED.h>
#include <math.h>
#include "dashspin.h"
#include "gamma.h";

#define LED_PIN 13

#define POT1_PIN 0
#define POT2_PIN 1

#define BUTTON1_PIN 12
#define BUTTON2_PIN 13

#define DEBUG 1

#define GAMMA 0

CRGB leds[NUM_LEDS];
int cycleSteps = NUM_LEDS * 1024;
  
#define EFFECT_TYPE_DASHPIN 0

int effectConfig[][2] = {
  {EFFECT_TYPE_DASHPIN, 1}
};

//TotallyLegitController tlc;
//Dashspin dashspin1(1, .1);

int offset = 0;
int mode = 0;
float velocity = 0;
float twoPercent = 0;
float secondPercent = .5;
int lastButtonState = 0;

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(9600);
  Serial.print("Starting...");

  pinMode(BUTTON1_PIN, INPUT); 
}

void loop() {

  float MAX_VELOCITY = NUM_LEDS;
  
  int buttonState[2] = {
    { 
      digitalRead(BUTTON1_PIN), 
      digitalRead(BUTTON2_PIN),
    }
  }

  if (button1State != lastButtonState) {
    lastButtonState = button1State;
    if (button1State == 1) {
      // It's clicked.
      if (mode == 1) {
        mode = 0;
      } else {
        mode = 1;
      }
    }
  }
    
  float pot1Percent = 1-(float) analogRead(POT1_PIN) / 1024;

  CRGB cleds[NUM_LEDS];

  if (mode == 0) {
    velocity =  ((float)(pot1Percent-.5) * MAX_VELOCITY )*.1;
  }
  
  if (velocity > MAX_VELOCITY) {
    velocity = MAX_VELOCITY /2;
  }
  
  int nextOffset = velocity + offset;

  if (nextOffset > 0) {
     offset = nextOffset % cycleSteps;
  } else {
     offset = cycleSteps - (nextOffset % cycleSteps);
  }
  
  float basePercent = (float) offset / cycleSteps;
  
  if (mode == 1) {
    secondPercent = pot1Percent;
  }
  
  dashspin1.loop(cleds, basePercent,secondPercent);
  
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
