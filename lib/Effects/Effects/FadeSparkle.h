

uint8_t lastSparkleRefreshMs = 0;
uint8_t lastFadeRefreshMs = 0;

void fadeSparkleEffectLoop(struct EffectState *effectState, bool isFadeDown)
{

  const uint8_t sparkleRefreshHz = config.LED_CNT * 5;
  const uint8_t fadeRefreshHz = sparkleRefreshHz;
  const uint16_t fadeSparkleLoopMs = 600000;
  const uint8_t loopPosition = effectState->loopPosition % fadeSparkleLoopMs;

  const uint8_t hue1 = 128;
  const uint8_t hue2 = 224;

  const float loopPercent = float(loopPosition) / fadeSparkleLoopMs;
  const uint8_t hue = hue1 + (float(hue2 - hue1) * abs((loopPercent / .5) - 1.0));

  const int fadeAmt = 1;

  if (millis() > lastSparkleRefreshMs + (1000 / sparkleRefreshHz))
  {

    const uint8_t blinkIdx = notRandom(0, config.LED_CNT - 1, effectState->loopPosition);

    leds[blinkIdx] = CHSV(hue, 255, 255);

    lastSparkleRefreshMs = millis();
  }

  if (millis() > lastFadeRefreshMs + (1000 / fadeRefreshHz))
  {
    if (isFadeDown)
    {
      fadeDown(fadeAmt);
    }
    else
    {
      fadeUp(fadeAmt);
    }
    lastFadeRefreshMs = millis();
  }

  copyLedsWithOffsetGamma();
}
