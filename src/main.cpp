#include <SPI.h>
#include <Config.h>
#include <Effects.h>
EffectState currentEffectState;
#include <Transmitter.h>

bool lastButtonState = false; // Default is not pressed.
int lastRecievedOffset = 0;

void setButtonState()
{
  // Button Logic
  boolean currentButtonState = !digitalRead(EFFECT_BUTTON_PIN);

  if (lastButtonState && currentButtonState != lastButtonState)
  {
    // BUTTON WAS CLICKED
    Serial.println("Click");

    currentEffectState.activeEffect = (currentEffectState.activeEffect + 1) % EFFECT_CNT;
    currentEffectState.sourceTransmitterId = currentEffectState.transmitterId; // Set it to us, this ain't a relay.
    transmitEffectState(&currentEffectState);                                  // Force a transmission loop
  }

  lastButtonState = currentButtonState;
}

void setup()
{

  Serial.begin(9600);
  randomSeed(analogRead(0));

  effectSetup();

  pinMode(EFFECT_BUTTON_PIN, INPUT);
  digitalWrite(EFFECT_BUTTON_PIN, HIGH);

  transmitterSetup();

  Serial.print("### SETUP COMPLETE for transmitter id: ");
  Serial.println(currentEffectState.transmitterId);
}

void loop()
{
  currentEffectState.loopPosition = (millis() + effectLoopClockOffset) % EFFECT_LOOP_MS;

  if (hasGottenSync)
  {
    // Button doesn't work until we get button sync
    setButtonState();
  }

  // Let's check for a new effect loop
  transmitterReceiveLoop(&currentEffectState);
  transmitterTransmitLoop(&currentEffectState);

  effectLoop(&currentEffectState);
}
