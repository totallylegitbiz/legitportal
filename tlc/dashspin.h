
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
    }

    void loop(CRGB cleds[NUM_LEDS], float basePercent) {

      int offset = floor(basePercent * NUM_LEDS);
      
      for (int i = 0; i < NUM_LEDS; i++) {
        cleds[i] = CHSV(0, 0, 0);
      }
      
      for (int j = -1; j < _dashCount; j++) {
          int centerLed = (((NUM_LEDS / _dashCount) * j) + offset)  % NUM_LEDS;
          int hue = (float)centerLed/NUM_LEDS * 255;
          
          cleds[centerLed] = CHSV(hue, 255, 255);

      }
      
    }
};
