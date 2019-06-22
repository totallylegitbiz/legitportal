
class Dashspin {
   private:
    int _dashCount;
    float _dashWidthPercent;
    int _ledWidth;
   public:
    Dashspin(int dashCount, float dashWidthPercent) {
      _dashCount = dashCount;
      _dashWidthPercent = dashWidthPercent;
      _ledWidth = dashWidthPercent * NUM_LEDS;
    }

    void loop(CRGB cleds[NUM_LEDS], float basePercent) {

     Serial.print("loop\n");
     Serial.print(":_ledWidth:");
     Serial.print(_ledWidth);
     Serial.print("\n");

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
      
      float offset = basePercent * NUM_LEDS;
 
      for (int j = 0; j < _dashCount; j++) {
       
        int dashOffset = ((float) j / _dashCount) * NUM_LEDS;

        for (int i = 0; i < _ledWidth - 1; i++) {

          int activeLed = (int) ( i + dashOffset + offset) % NUM_LEDS;
          float activeLedPercent = ( i + dashOffset + offset) - floor( i + dashOffset + offset);
          
          if (activeLed < 0) {
            continue;
          }

          int hue = (float) activeLed / NUM_LEDS * 255;

          int intensityFloor = intensityMap[i];
          int intensityCeil = intensityMap[i+1];
  
           
          int activePixel = floor(activeLed);

          int intensity = 
            ((float) intensityCeil* ((float)1-activeLedPercent)) 
            + 
            ((float)intensityFloor * activeLedPercent);

          int colorOffset = -1 * floor(offset);
          int offSetHue = colorOffset;
          cleds[activePixel] = CHSV(hue, 255,intensity);
          
        }
      }
    }
};
