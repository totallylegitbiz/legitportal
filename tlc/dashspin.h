
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

    for (int i = 0; i < NUM_LEDS; i++) {
        int hue = (float)i/NUM_LEDS * 255;

        cleds[i] = CHSV(255, 255, 0);

        for (int j = 0 - _dashCount; j < _dashCount; j++) {

           float dashOffset = ((float) j / _dashCount) * NUM_LEDS;
           float selectedLed = (basePercent * NUM_LEDS) + dashOffset;
  
           if (abs(i-selectedLed) < (_ledWidth / 2)) {
            float distPercent = abs(i-selectedLed) / (_ledWidth / 2);
            float intensity = cos(PI * .5 * distPercent) * 255;
            cleds[i] = CHSV(hue, 255-(intensity/2), intensity*.75);
            }
        }  
       }
       
//    for (int i = 0; i < NUM_LEDS; i++) {
//        int hue = (float)i/NUM_LEDS * 255;
//
//        cleds[i] = CHSV(255, 255, 0);
//
//        for (int j = 0 - dashCount; j < dashCount; j++) {
//
//           float dashOffset = ((float) j / dashCount) * NUM_LEDS;
//           float selectedLed = (basePercent * NUM_LEDS) + dashOffset;
//  
//           if (abs(i-selectedLed) < (ledWidth / 2)) {
//            float distPercent = abs(i-selectedLed) / (ledWidth / 2);
//            float intensity = cos(PI * .5 * distPercent) * 255;
//            cleds[i] = CHSV(hue, 255-(intensity/2), intensity*.75);
//            }
//        }  
//       }
    }
};
