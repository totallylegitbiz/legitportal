#include <EffectTypes.h>
#include <FastLED.h>

void hueBarsEffectLoop(struct EffectState *effectState, unsigned int loopMs)
{

  const float loopPercent = float(effectState->loopPosition % loopMs) / loopMs;
  const float huePercent = float(effectState->loopPosition) / EFFECT_LOOP_MS;

  const unsigned int lines = LED_CNT / 30;

  const int h = huePercent * 255;
  const int offset = loopPercent * LED_CNT;

  // zeroOutStrip();
  for (int i = 0; i < lines; i++)
  {
    const float linePercent = float(i) / lines;
    const int baseIdx = (int((linePercent)*LED_CNT) + offset) % 255;

    Serial.print("baseIdx: ");
    Serial.println(baseIdx);
    drawDash(2, baseIdx, CHSV(20, 255, 255));
  }

  copyLedsWithOffset();
}
