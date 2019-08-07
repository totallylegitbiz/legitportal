
void drawDash(int width, int idx, CRGB color)
{
  for (int i = (-width  / 2); i < (width / 2); i++)
  {
    const unsigned int currIdx = (idx + i) % LED_CNT;    
    leds[currIdx] += color;    
  }
}

/* fade() 0-255 amount to keep (255 changes nothing) */
void fadeDown(unsigned int by) {
  for (int i = 0; i < LED_CNT; i++)
  {
    leds[i].subtractFromRGB( by );
  }
}
void fadeUp(unsigned int by) {
  for (int i = 0; i < LED_CNT; i++)
  {
    leds[i].addToRGB( by );
  }
}