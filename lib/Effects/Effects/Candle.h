#include <EffectTypes.h>
#include <RealCandle.h>

const CRGB RED = CRGB(255, 0, 0);
const CRGB BLUE = CRGB(255, 0, 0);

void candleEffectLoop(struct EffectState *effectState)
{

    const candleSize = candle2Size;
    const candle = candle2;

    const unsigned int loopMs = 60000; // 60 second loop
    const unsigned int loopPosition = effectState->loopPosition % loopMs;
    const float loopPercent = float(loopPosition) / loopMs;
    const unsigned int ledOffset = LED_CNT * (float(loopPosition) / loopMs);

    const unsigned int hue = 20;
    const unsigned int sat = 225;
    const unsigned int intensity = abs(cos(PI * .5 * loopPercent * 2) * 255);

    const int idx = loopPercent * candleSize;

    for (int i = 0; i < LED_CNT; i++)
    {
      const unsigned int ledOffset = ledOffsets[i];
      const int candleIdx = (idx + ledOffset) % candleSize;
      const uint8_t intensity = pgm_read_byte(&candle[candleIdx]);

      leds[i] = CHSV(hue, sat, intensity);
    }
}





