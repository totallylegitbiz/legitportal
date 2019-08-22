void policeEffectLoop(struct EffectDataPacket *effectState)
{

  const uint8_t shortDelay = 20;
  const uint8_t medDelay = 100;
  const uint16_t longDelay = 500;

  const uint8_t blueHue = 160;
  const uint8_t redHue = 0;

  const uint8_t lowIntensity = 10;
  const uint8_t highIntensity = 255;

  const bool side = effectState->transmitterId % 2;

  const bool isEven = side % 2;
  const uint8_t hue = isEven ? blueHue : redHue;

  const CHSV stepColors[] = {
      CHSV(hue, 255, lowIntensity),
      CHSV(hue, 255, highIntensity),
      CHSV(hue, 255, lowIntensity),
      CHSV(hue, 255, highIntensity),
      CHSV(hue, 255, lowIntensity),
      CHSV(hue, 255, lowIntensity),
  };

  if (isEven)
  {
    uint16_t stepDelays[] = {
        0,
        shortDelay,
        medDelay,
        shortDelay,
        longDelay,
        shortDelay + medDelay + shortDelay + longDelay,
    };

    patternLoopEffectLoop(effectState, stepDelays, stepColors, 6);
  }
  else
  {

    uint16_t stepDelays[] = {
        shortDelay + medDelay + shortDelay + longDelay,
        shortDelay,
        medDelay,
        shortDelay,
        longDelay,
        0,
    };

    patternLoopEffectLoop(effectState, stepDelays, stepColors, 6);
  }
}