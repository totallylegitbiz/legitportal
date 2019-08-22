

void groupSolidColorEffectLoop(struct EffectDataPacket *effectState, uint8_t colorCnt)
{

  const uint8_t colorIdx = effectState->transmitterId % colorCnt;

  const uint8_t hue = floor(colorIdx) / colorCnt * 255;

  solidEffectLoop(effectState, CHSV(hue, 255, 255));
}