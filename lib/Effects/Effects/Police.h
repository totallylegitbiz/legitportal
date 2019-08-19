void policeEffectLoop(struct EffectDataPacket *effectState)
{

  const uint8_t shortDelay = 20;
  const uint8_t medDelay = 100;
  const uint16_t longDelay = 500;

  const uint8_t blueHue = 160;
  const uint8_t redHue = 0;

  const bool side = effectState->transmitterId % 2;

  uint16_t stepDelays[] = {
      shortDelay,
      medDelay,
      shortDelay,
      longDelay,
      shortDelay,
      medDelay,
      shortDelay,
      longDelay,
  };

  if (side == 0)
  {
    const CHSV stepColors[] = {
        CHSV(redHue, 255, 255),
        CHSV(0, 255, 0), // Black
        CHSV(redHue, 255, 255),
        CHSV(0, 255, 0), // Black
        CHSV(blueHue, 255, 255),
        CHSV(0, 255, 0), // Black
        CHSV(blueHue, 255, 255),
        CHSV(0, 255, 0), // Black
    };

    patternLoopEffectLoop(effectState, stepDelays, stepColors, 8);
  }
  else
  {

    const CHSV stepColors[] = {
        CHSV(blueHue, 255, 255),
        CHSV(0, 255, 0), // Black
        CHSV(blueHue, 255, 255),
        CHSV(0, 255, 0), // Black
        CHSV(redHue, 255, 255),
        CHSV(0, 255, 0), // Black
        CHSV(redHue, 255, 255),
        CHSV(0, 255, 0), // Black
    };

    patternLoopEffectLoop(effectState, stepDelays, stepColors, 8);
  }
}
