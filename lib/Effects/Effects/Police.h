#include <EffectTypes.h>

void policeEffectLoop(struct EffectState *effectState)
{
  
  const unsigned int stepDelays[] = { 
    20, 
    100, 
    20, 
    40,
    20, 
    100, 
    20, 
    300,
  };

  const CHSV stepColors[] = {
    CHSV(0,255,255), 
    CHSV(0,255,0), // Black 
    CHSV(0,255,255), 
    CHSV(0,255,0), // Black 
    CHSV(170,255,255), 
    CHSV(0,255,0), // Black 
    CHSV(170,255,255), 
    CHSV(0,255,0), // Black 
  };

  patternLoopEffectLoop(effectState, stepDelays, stepColors, 8);
}





