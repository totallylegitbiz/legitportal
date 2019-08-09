#include <EffectTypes.h>

class RunEvery
{
public:
  RunEvery(unsigned int everyMs);
  boolean shouldRun();

private:
  unsigned int _everyMs;
  unsigned long _lastRunMs;
};

RunEvery::RunEvery(unsigned int everyMs)
{
  _everyMs = everyMs;
  _lastRunMs = 0;
}

bool RunEvery::shouldRun()
{
  if (_lastRunMs + _everyMs < millis())
  {
    _lastRunMs = millis() - (millis() % _everyMs);
    return true;
  }

  return false;
}

RunEvery runEvery(20);

const int SPARKLE_BATCH_CNT = LED_CNT / 2;

void sparkleEffectLoop(struct EffectState *effectState, int sat)
{

  const unsigned int loopMs = 60 * 1000;
  const unsigned int loopPosition = effectState->loopPosition % loopMs;

  if (runEvery.shouldRun())
  {
    for (int i = 0; i < SPARKLE_BATCH_CNT; i++)
    {
      const int idx = notRandom(0, LED_CNT - 1, loopPosition * i);
      const int hue = notRandom(0, 255, loopPosition * i);
      leds[idx] = CHSV(hue, sat, 255);
    }

    for (int i = 0; i < SPARKLE_BATCH_CNT / 3; i++)
    {
      const int blankIdx = notRandom(0, (LED_CNT - 1) * 5, loopPosition * i);

      leds[blankIdx % LED_CNT] = CHSV(0, 255, 0);
    }
  }

  copyLedsWithOffset();
}