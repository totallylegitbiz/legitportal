#include <SPI.h>
#include <Config.h>
#include <Effects.h>
EffectState currentEffectState;
Config config = getConfig();

#include <Transmitter.h>

bool lastButtonState = false; // Default is not pressed.
int lastRecievedOffset = 0;

void setButtonState()
{
  // Button Logic
  boolean currentButtonState = !digitalRead(config.EFFECT_BUTTON_PIN);

  if (lastButtonState && currentButtonState != lastButtonState)
  {
    // BUTTON WAS CLICKED
    Serial.println("Click");

    currentEffectState.activeEffect = (currentEffectState.activeEffect + 1) % EFFECT_CNT;
    currentEffectState.age = 0;
    lastDataCreationTs = 0;
    currentEffectState.sourceTransmitterId = currentEffectState.transmitterId; // Set it to us, this ain't a relay.
    transmitEffectState(&currentEffectState);                                  // Force a transmission loop
  }

  lastButtonState = currentButtonState;
}

void setup()
{

  // Setup config.

  Serial.begin(9600);
  randomSeed(analogRead(0));

  effectSetup();

  pinMode(config.EFFECT_BUTTON_PIN, INPUT);
  digitalWrite(config.EFFECT_BUTTON_PIN, HIGH);

  pinMode(config.RED_LED_PIN, OUTPUT);
  pinMode(config.BLUE_LED_PIN, OUTPUT);
  pinMode(config.GREEN_LED_PIN, OUTPUT);

  pinMode(config.SENSOR_PIN, INPUT);

  transmitterSetup();

  Serial.print("### SETUP COMPLETE for transmitter id: ");
  Serial.println(currentEffectState.transmitterId);
}

void diagnoticModeLoop()
{

  Serial.print("DIP: ");
  Serial.println(getDipValue(config));

  Serial.println("RED");
  analogWrite(config.RED_LED_PIN, 255);
  recievedStatusEffect(CRGB(255, 0, 0), 500);

  analogWrite(config.RED_LED_PIN, 0);
  analogWrite(config.BLUE_LED_PIN, 0);

  Serial.println("RED");
  analogWrite(config.GREEN_LED_PIN, 255);
  recievedStatusEffect(CRGB(0, 255, 0), 500);
  analogWrite(config.GREEN_LED_PIN, 0);

  Serial.println("BLUE");
  analogWrite(config.BLUE_LED_PIN, 255);
  recievedStatusEffect(CRGB(0, 0, 255), 500);
  analogWrite(config.BLUE_LED_PIN, 0);
}

void loop()
{

  if (config.DIAGNOSTIC_MODE)
  {
    diagnoticModeLoop();
    return;
  }

  currentEffectState.loopPosition = (millis() + effectLoopClockOffset) % config.EFFECT_LOOP_MS;

  if (hasGottenSync)
  {
    // Button doesn't work until we get button sync
    setButtonState();
  }

  effectLoop(&currentEffectState, &config);

  FastLED.show();
}
