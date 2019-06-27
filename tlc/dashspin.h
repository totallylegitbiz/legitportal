
class Dashspin {
   private:
    int _dashCount;
    
   public:
    Dashspin(int dashCount, float dashWidthPercent) {
      _dashCount = dashCount;
    }

    void loop(CRGB cleds[NUM_LEDS], float basePercent, float twoPercent) {


      float dashWidthPercent = twoPercent * 1/_dashCount;
      int _ledWidth = dashWidthPercent * NUM_LEDS;
      
      int intensityMap[_ledWidth];

      for (int i = 0; i < NUM_LEDS; i++) {
        cleds[i] = CHSV(0, 0, 0);
      }

      for (int i = 0; i < _ledWidth; i++) {
        
        int distanceFromCenter = abs(i - (_ledWidth/2)); 
        float distPercent = (float) distanceFromCenter / (_ledWidth / 2);
        float intensity = cos(PI * .5 * distPercent) * 255;

        intensityMap[i] = intensity;

      }
      
//      float offset = basePercent * NUM_LEDS;
      float offset = basePercent  * NUM_LEDS * 100;
      float realOffst = basePercent * NUM_LEDS;

 
      for (int j = 0; j < _dashCount; j++) {
       
        int dashOffset = ((float) j / _dashCount) * NUM_LEDS;

        for (int i = 0; i < _ledWidth - 1; i++) {

          int activeLed = (int) ( i + dashOffset + offset) % NUM_LEDS;
          float activeLedPercent = ( i + dashOffset + offset) - floor( i + dashOffset + offset);
          
          if (activeLed < 0) {
            continue;
          }

          int hue = (float) (activeLed + realOffst) / NUM_LEDS * 255;

          int intensityFloor = intensityMap[i];
          int intensityCeil = intensityMap[i+1];
  
          int activePixel = floor(activeLed);

          int intensity = 
            ((float) intensityCeil* ((float)1-activeLedPercent)) 
            + 
            ((float)intensityFloor * activeLedPercent);

          
          cleds[activePixel] = CHSV(hue  % 255, 0,intensity);
          
        }
      }
    }
};
