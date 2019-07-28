#include <SPI.h>
#include <Config.h>
#include <Effects.h>

EffectState currentEffectState;

boolean lastButtonState = 0; // Default is not pressed.

void setup()
{

  Serial.begin(9600);
  effectSetup();

  pinMode(EFFECT_BUTTON_PIN, INPUT);
  digitalWrite(EFFECT_BUTTON_PIN, HIGH);
}

int effectLoopIntervalMs = 5000;
bool effectLoopLastOffset = effectLoopIntervalMs;

int lastRecievedOffset = 0;
int effectLoopClockOffset = 0;

void loop()
{

  // Button Logic
  boolean currentButtonState = !digitalRead(EFFECT_BUTTON_PIN);

  if (lastButtonState && currentButtonState != lastButtonState)
  {
    // BUTTON WAS CLICKED
    Serial.println("Click");

    currentEffectState.activeEffect = (currentEffectState.activeEffect + 1) % EFFECT_CNT;
  }
  lastButtonState = currentButtonState;

  currentEffectState.loopPosition = millis() % EFFECT_LOOP_MS;

  effectLoop(&currentEffectState);

  // const int effectLoopOffset = (millis() + effectLoopClockOffset) % effectLoopIntervalMs;
  // const float effectLoopOffsetPercent = float(effectLoopOffset) / effectLoopIntervalMs;

  // for (int i = 0; i < LED_CNT; i++)
  // {
  //   // if (i < (LED_CNT * effectLoopOffsetPercent))
  //   // {
  //   //   leds[i] = CHSV(50, 100, 100);
  //   // }
  //   // else
  //   // {

  //   // }

  //   leds[i] = CHSV(effectLoopOffsetPercent * 255, 255, 70);
  // }
  // FastLED.show();
}