#include <FastLED.h>
#include <math.h>
#define LED_PIN     13
#define NUM_LEDS    150
#define POT1_PIN    0
#define BUTTON1_PIN  12
#define DEBUG 1

CRGB leds[NUM_LEDS];

class TotallyLegitController {
   public:
    void setup() {
      Serial.print("set...");
    }

    void loop(CRGB cleds[NUM_LEDS], float basePercent) {


      float widthPercent = .15;
      int dashNum = 4;

      int dashOffset = dashNum;
      float ledWidth = widthPercent * NUM_LEDS;
    
     
      for (int i = 0; i < NUM_LEDS; i++) {
        int hue = (float)i/NUM_LEDS * 255;

        cleds[i] = CHSV(255, 255, 0);

        for (int j = 0 - dashOffset; j < dashNum + dashOffset; j++) {

           float dashOffset = ((float) j / dashNum) * NUM_LEDS;
           float selectedLed = (basePercent * NUM_LEDS) + dashOffset;
  
           if (abs(i-selectedLed) < (ledWidth / 2)) {
            float distPercent = abs(i-selectedLed) / (ledWidth / 2);
            float intensity = cos(PI * .5 * distPercent) * 255;
            cleds[i] = CHSV(hue, 255-(intensity/2), intensity*.75);
            }
        }
          
       }
       
//       int hue = (float)i/NUM_LEDS * 255;
//         
//        // Default off.
//        cleds[i] = CHSV(hue, 255, 0);  
//
//        abs(i-selectedLed) < (ledWidth / 2)
//        
//        if (abs(i-selectedLed) < (ledWidth / 2)) {
//
//          float distPercent = abs(i-selectedLed) / (ledWidth / 2);
//          float intensity = cos(PI * .5 * distPercent) * 255;
//          
//          cleds[i] = CHSV(hue, 255, 255);  
//        }

    }
};

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
