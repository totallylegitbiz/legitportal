#include <EffectTypes.h>

const unsigned int sparkleRefreshHz = LED_CNT * 5 ;
unsigned int lastSparkleRefreshMs = 0;

const unsigned int fadeRefreshHz = sparkleRefreshHz;
unsigned int lastFadeRefreshMs = 0;

const unsigned int fadeSparkleLoopMs = 600000;

void fadeSparkleEffectLoop(struct EffectState *effectState, bool isFadeDown)
{

    const unsigned int loopPosition = effectState->loopPosition % fadeSparkleLoopMs;
   
    const unsigned int hue1 = 128;  
    const unsigned int hue2 = 224;  
    const float loopPercent = float(loopPosition) / fadeSparkleLoopMs;

    const unsigned int hue = hue1 + (float(hue2-hue1)*abs((loopPercent  / .5)-1.0));

    const int fadeAmt = 1;

    if (millis() > lastSparkleRefreshMs + (1000 / sparkleRefreshHz))
    {

      const unsigned int blinkIdx = notRandom(0,LED_CNT - 1, effectState->loopPosition);
      
      leds[blinkIdx] = CHSV(hue, 255,255);
  
      lastSparkleRefreshMs = millis();
    }

    if (millis() > lastFadeRefreshMs + (1000 / fadeRefreshHz))
    {
      if (isFadeDown) {
        fadeDown(fadeAmt);
      } else {
        fadeUp(fadeAmt);
      }
      lastFadeRefreshMs =  millis();
    }


    copyLedsWithOffsetGamma();
 

}




