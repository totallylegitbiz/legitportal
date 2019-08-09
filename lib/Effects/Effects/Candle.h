#include <EffectTypes.h>
#include <RealCandle.h>

void candleEffectLoop(struct EffectState *effectState, unsigned int steps)
{
  const unsigned int loopMs = 60000; // 60 second loop
  const unsigned int loopPosition = effectState->loopPosition % loopMs;
  const float loopPercent = float(loopPosition) / loopMs;
  const unsigned int ledOffset = LED_CNT * (float(loopPosition) / loopMs);

  const unsigned int hue = 20;
  const unsigned int sat = 225;
  const unsigned int intensity = abs(cos(PI * .5 * loopPercent * 2) * 255);

  const int idx = loopPercent * candle2Size;

  const unsigned int stepLoopMs = 500;
  const unsigned int stepLoopPosition = effectState->loopPosition % stepLoopMs;
  const float stepLoopPercent = float(stepLoopPosition) / stepLoopMs;

  const unsigned int step = stepLoopPercent * steps;

  for (int i = 0; i < LED_CNT; i++)
  {
    const unsigned int ledOffset = ledOffsets[i];
    const int candleIdx = (idx + ledOffset) % candle2Size;
    const uint8_t intensity = pgm_read_byte(&candle2[candleIdx]);

    if (i % steps == step)
    {
      leds[i] = CHSV(hue, sat, intensity);
    }
    else
    {
      leds[i] = CHSV(hue, sat, 0);
    }
  }

  copyLedsWithOffset();
}
