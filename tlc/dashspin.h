
class Dashspin {
   private:
    int _dashCount;
    float _dashWidthPercent;
    float _ledWidth;
   public:
    Dashspin(int dashCount, float dashWidthPercent) {
      _dashCount = dashCount;
      _dashWidthPercent = dashWidthPercent;
      _ledWidth = dashWidthPercent * NUM_LEDS;
      _dashAreaWidth = ;
    }

    void loop(CRGB cleds[NUM_LEDS], float basePercent) {

      int offset = floor(basePercent * NUM_LEDS);

      
      for (int i = 0; i < NUM_LEDS; i++) {
        cleds[i] = CHSV(0, 0, 0);
      }

      for (int j = -1; j < _dashCount; j++) {
        for (int i = 0; i < _ledWidth; i++) {
          
//          int activeLed = (((NUM_LEDS / _dashCount) * j) + offset)  % NUM_LEDS;
          int activeLed = i+(j
          
//          float centerLed = activeLed + ((float)_ledWidth / 2);
//
//          float distPercent = abs(activeLed-activeLed) / (_ledWidth / 2);
//          float intensity = cos(PI * .5 * distPercent) * 255;
//
//          Serial.print("activeLed:");
//          Serial.print(activeLed);
//          Serial.print(":intensity:");
//          Serial.print(intensity);
//          Serial.print("\n");
  
          int hue = (float)activeLed/NUM_LEDS * 255;

  
//          cleds[activeLed] = CHSV(hue, 255-(intensity/2), intensity*.75);
          cleds[activeLed] = CHSV(hue, 255,255);

        }
      }
    }
};
