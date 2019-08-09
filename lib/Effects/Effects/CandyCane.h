

void candyCaneEffectLoop(struct EffectState *effectState)
{
  const unsigned int loopMs = 1000;
  const unsigned int loopPosition = effectState->loopPosition % loopMs;

  // drawDash((LED_CNT/2)+1, 0, CHSV(0, 255, 255));
  drawDash(config.LED_CNT / 2, config.LED_CNT / 2, CRGB(0, 220, 255));
  // drawDash(LED_CNT/2,5, CRGB(0, 220, 255));

  copyLedsWithOffset();
}