

void candyCaneEffectLoop(struct EffectDataPacket *effectState)
{

  // drawDash((LED_CNT/2)+1, 0, CHSV(0, 255, 255));
  drawDash(LED_CNT / 2, LED_CNT / 2, CRGB(0, 220, 255));
  // drawDash(LED_CNT/2,5, CRGB(0, 220, 255));

  copyLedsWithOffset();
}