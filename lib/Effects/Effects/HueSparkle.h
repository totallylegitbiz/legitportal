
#include <EffectTypes.h>

void hueSparkleEffectLoop(struct EffectState *effectState)
{
  
   for (int i = 0; i < LED_CNT; i++)
  {

    leds[i] = CHSV(140, 244, 255);
  }

}





