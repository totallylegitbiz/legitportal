
#include <RealCandle.h>

void candleEffectLoop(struct EffectDataPacket *effectState, uint8_t steps)
{
  const uint16_t loopMs = 60000; // 60 second loop
  const uint8_t loopPosition = effectState->loopPosition % loopMs;
  const float loopPercent = float(loopPosition) / loopMs;

  const uint8_t hue = 20;
  const uint8_t sat = 225;

  const int idx = loopPercent * candle2Size;

  const uint8_t stepLoopMs = 500;
  const uint8_t stepLoopPosition = effectState->loopPosition % stepLoopMs;
  const float stepLoopPercent = float(stepLoopPosition) / stepLoopMs;

  const uint8_t step = stepLoopPercent * steps;

  for (uint16_t i = 0; i < config.LED_CNT; i++)
  {
    const uint8_t ledOffset = notRandom(0, config.LED_CNT - 1, i);
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
