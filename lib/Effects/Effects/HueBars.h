#include <EffectTypes.h>
#include <FastLED.h>

void hueBarsEffectLoop(struct EffectState *effectState, unsigned int loopMs) {

  const float loopPercent = float(effectState->loopPosition % loopMs) / loopMs;
  const float huePercent = float(effectState->loopPosition) / EFFECT_LOOP_MS;

  // const unsigned int lines = 1;

  // const int h = huePercent * 255;
  // const int offset = loopPercent * LED_CNT;

  // for (int i = 0; i < lines;i++) {
  //   const float linePercent = float(i)/lines;
  //   const int baseIdx = (int((linePercent)*LED_CNT)+offset)%255;
  //   drawDash(2, baseIdx, CHSV(int(h+(linePercent*255))%255, 255, 255));
  // }

  copyLedsWithOffset();
}
