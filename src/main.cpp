#include <SPI.h>
#include <Config.h>
#include <Effects.h>
#include <Transmitter.h>

EffectState currentEffectState;

boolean lastButtonState = 0; // Default is not pressed.

void setup()
{

  Serial.begin(9600);
  effectSetup();

  pinMode(EFFECT_BUTTON_PIN, INPUT);
  digitalWrite(EFFECT_BUTTON_PIN, HIGH);

  transmitterSetup();

  Serial.println("@@ SETUP COMPLETE");
}

// int effectLoopIntervalMs = 5000;
// bool effectLoopLastOffset = effectLoopIntervalMs;

int lastRecievedOffset = 0;
unsigned int effectLoopClockOffset = 0;

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

  //
  currentEffectState.loopPosition = (millis() + effectLoopClockOffset) % EFFECT_LOOP_MS;

  // Let's deal with transmission stuff;

  transmitterLoop(&currentEffectState);
  effectLoop(&currentEffectState);
}