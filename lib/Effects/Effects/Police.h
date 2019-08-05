#include <EffectTypes.h>
#include <RealCandle.h>

const CRGB RED = CRGB(255, 0, 0);
const CRGB BLUE = CRGB(255, 0, 0);

void policeEffectLoop(struct EffectState *effectState)
{
    const unsigned int loopMs = 60000; // 60 second loop
    const unsigned int loopPosition = effectState->loopPosition % loopMs;
    const float loopPercent = float(loopPosition) / loopMs;
    const int ledOffset = LED_CNT * (float(loopPosition) / loopMs);

    const int hue = 20;
    const int sat = 225;

    const unsigned int intensity = abs(cos(PI * .5 * loopPercent * 2) * 255);

    const int idx = loopPercent * candle2Size;

    for (int i = 0; i < LED_CNT; i++)
    {
      const unsigned int ledOffset = ledOffsets[i];
      const int candleIdx = (idx + ledOffset) % candle2Size;
      const uint8_t intensity = pgm_read_byte(&candle2[candleIdx]);

      leds[i] = CHSV(hue, sat, intensity);
    }

    // for (int i = 0; i < LED_CNT; i++) {

    //   const unsigned int ledOffset = ledOffsets[i];
    //   // const int candleIdx = (idx + ledOffset) % candle1Size;
    //   const unsigned int candleIdx = idx;

    //   if (candleIdx >= candle1Size || candleIdx < 0)
    //   {
    //     Serial.print("candleIdx");
    //     Serial.println(candleIdx);
    //   }


    // // const uint8_t intensity = pgm_read_byte(&candle1[candleIdx]);

    //   leds[i] = CHSV(0, 255, intensity);


    // // int hueRange[] = {40, 30};
    // // int valRange[] = {10, 50};
    // // int satRange[] = {255, 214};

    // // // int hueRange[] = {255, 0};
    // // // int valRange[] = {10, 50};
    // // // int satRange[] = {255, 214};

    // // float intensityPercent = float(intensity) / 255;

    // // int hue = (float(hueRange[1] - hueRange[0]) * intensityPercent) + hueRange[0];
    // // int val = (float(valRange[1] - valRange[0]) * intensityPercent) + valRange[0];
    // // int sat = (float(satRange[1] - satRange[0]) * intensityPercent) + satRange[0];

    // // leds[i] = CHSV(hue, sat, val);

    

  // }/

  // leds[0] = CHSV(60, 255, ((candleLoopOffset / 2) % 255));
  // FastLED.show();

}





