
void drawDash(int width, int idx, CRGB color)
{
  for (int i = (-width  / 2); i < (width / 2); i++)
  {
    const unsigned int currIdx = (idx + i) % LED_CNT;

    Serial.println(currIdx);
    
    leds[currIdx] += color;    
  }
}