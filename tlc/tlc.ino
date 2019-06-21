#include <FastLED.h>
#include <math.h>
#define LED_PIN 13
#define NUM_LEDS 150
#define POT1_PIN 0
#define BUTTON1_PIN 12
#define DEBUG 1

CRGB leds[NUM_LEDS];

TotallyLegitController tlc;

int offset = 0;

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(9600);
  Serial.print("Starting...");

  pinMode(BUTTON1_PIN, INPUT);
  tlc.setup();
  
}


void loop() {


  int buttonState = digitalRead(BUTTON1_PIN);

  Serial.print("buttonState...");
  
  Serial.print(buttonState);
  
  Serial.print("\n");
  float pot1Percent = (float) analogRead(POT1_PIN) / 1024;
  
  int cycleMs = 1 * 1000;
    int offset = millis() % cycleMs;
  
//  int offset = 150 * pot1Percent;
  float basePercent = (float) offset / cycleMs;
  
  CRGB cleds[NUM_LEDS];
  
  tlc.loop(cleds, basePercent);
  
  for (int i = 0; i < NUM_LEDS; i++) {
    
    leds[i] = cleds[i];
    
//    Serial.print(leds[i].r);
//    Serial.print("\n");
         
//         CHSV(hue, 255, 100);  

//    float ledPercent = (float) i / NUM_LEDS;
//    float offsetPercent = (ledPercent + basePercent);
     
//
//    Serial.print("basePercent:");
//    Serial.print(basePercent);
//    Serial.print(":i:");
//    Serial.print(i);
//    Serial.print("\n");
//    
//    float hue = (float) (ledPercent + basePercent) * 255;

//    Serial.print("hue:");
//    Serial.print(hue);
//    Serial.print("\n");
    
//    int hue = 255 * colorPercent;
//    Serial.print("hue:");
//    Serial.print(hue);  
//    Serial.print("....\n");
    
//    float hue =( (float) (i+offset) / NUM_LEDS) * 255;

    
//     leds[i] = CHSV(hue, 255, 100);  
    // CRGB(255,255, 255);

  }

  FastLED.show();
 
//  delay(1);
}
