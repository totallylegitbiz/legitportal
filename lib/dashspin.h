

class Dashspin
{
private:
  int _dashCount;

public:
  Dashspin(int dashCount, float dashWidthPercent)
  {
    _dashCount = dashCount;
  }

  void loop(CRGB cleds[LED_NUM], InputState *inputState)
  {

    for (int i = 0; i < LED_NUM; i++)
    {
      cleds[i] = CHSV(0, 255, 255);
    }

    //
    //      float dashWidthPercent = twoPercent * 1/_dashCount;
    //      int _ledWidth = dashWidthPercent * LED_NUM;
    //
    //      int intensityMap[_ledWidth];
    //
    //      for (int i = 0; i < LED_NUM; i++) {
    //        cleds[i] = CHSV(0, 0, 0);
    //      }
    //
    //      for (int i = 0; i < _ledWidth; i++) {
    //
    //        int distanceFromCenter = abs(i - (_ledWidth/2));
    //        float distPercent = (float) distanceFromCenter / (_ledWidth / 2);
    //        float intensity = cos(PI * .5 * distPercent) * 255;
    //
    //        intensityMap[i] = intensity;
    //
    //      }
    //
    ////      float offset = basePercent * LED_NUM;
    //      float offset = basePercent  * LED_NUM * 100;
    //      float realOffst = basePercent * LED_NUM;
    //
    //
    //      for (int j = 0; j < _dashCount; j++) {
    //
    //        int dashOffset = ((float) j / _dashCount) * LED_NUM;
    //
    //        for (int i = 0; i < _ledWidth - 1; i++) {
    //
    //          int activeLed = (int) ( i + dashOffset + offset) % LED_NUM;
    //          float activeLedPercent = ( i + dashOffset + offset) - floor( i + dashOffset + offset);
    //
    //          if (activeLed < 0) {
    //            continue;
    //          }
    //
    //          int hue = (float) (activeLed + realOffst) / LED_NUM * 255;
    //
    //          int intensityFloor = intensityMap[i];
    //          int intensityCeil = intensityMap[i+1];
    //
    //          int activePixel = floor(activeLed);
    //
    //          int intensity =
    //            ((float) intensityCeil* ((float)1-activeLedPercent))
    //            +
    //            ((float)intensityFloor * activeLedPercent);
    //
    //
    //          cleds[activePixel] = CHSV(hue  % 255, 255, intensity);
    //
    //        }
    //      }
  }
};
