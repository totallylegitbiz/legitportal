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

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);

  pinMode(PULSE_SENSOR_PIN, INPUT);

  // DIP
  pinMode(DIP_PIN_0, INPUT_PULLUP);
  pinMode(DIP_PIN_1, INPUT_PULLUP);
  pinMode(DIP_PIN_2, INPUT_PULLUP);
  pinMode(DIP_PIN_3, INPUT_PULLUP);

  transmitterSetup();

  Serial.print("### SETUP COMPLETE for transmitter id: ");
  Serial.println(currentEffectState.transmitterId);
}

unsigned int getDipValue()
{
  const bool d0 = digitalRead(DIP_PIN_0);
  const bool d1 = digitalRead(DIP_PIN_1);
  const bool d2 = digitalRead(DIP_PIN_2);
  const bool d3 = digitalRead(DIP_PIN_3);

  return !d0 + (!d1 * 2) + (!d2 * 4) + (!d3 * 8);
}
void diagnoticModeLoop()
{

  Serial.print("DIP: ");
  Serial.println(getDipValue());

  recievedStatusEffect(CRGB(255, 0, 0), 500);
  recievedStatusEffect(CRGB(0, 255, 0), 500);
  recievedStatusEffect(CRGB(0, 0, 255), 500);

  analogWrite(RED_LED_PIN, 255);
  Serial.println("STATUS LED: red");
  delay(500);

  Serial.println("STATUS LED: green");
  analogWrite(RED_LED_PIN, 0);
  analogWrite(GREEN_LED_PIN, 255);
  delay(500);

  Serial.println("STATUS LED: blue");
  analogWrite(GREEN_LED_PIN, 0);
  analogWrite(BLUE_LED_PIN, 255);
  delay(500);

  Serial.println("STATUS LED: white");
  analogWrite(RED_LED_PIN, 255);
  analogWrite(GREEN_LED_PIN, 255);
  analogWrite(BLUE_LED_PIN, 255);
  delay(500);

  analogWrite(RED_LED_PIN, 0);
  analogWrite(GREEN_LED_PIN, 0);
  analogWrite(BLUE_LED_PIN, 0);
}

void loop()
{

  if (DIAGNOSTIC_MODE)
  {
    diagnoticModeLoop();
    return;
  }

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
