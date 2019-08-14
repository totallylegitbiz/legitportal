

void policeEffectLoop(struct EffectDataPacket *effectState)
{

  const int shortDelay = 20;
  const int medDelay = 100;
  const int longDelay = 500;

  const int blueHue = 160;

  uint8_t stepDelays[] = {
      shortDelay,
      medDelay,
      shortDelay,
      longDelay,
      shortDelay,
      medDelay,
      shortDelay,
      longDelay,
  };

  const CHSV stepColors[] = {
      CHSV(0, 255, 255),
      CHSV(0, 255, 0), // Black
      CHSV(0, 255, 255),
      CHSV(0, 255, 0), // Black
      CHSV(blueHue, 255, 255),
      CHSV(0, 255, 0), // Black
      CHSV(blueHue, 255, 255),
      CHSV(0, 255, 0), // Black
  };

  patternLoopEffectLoop(effectState, stepDelays, stepColors, 8);
}
