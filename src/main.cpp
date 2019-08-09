#include <SPI.h>
#include <Config.h>

EffectState effectState;
Config config = getConfig();

#include <Effects.h>
#include <Transmitter.h>

CRGB cleds[MAX_LEDS];
CRGB leds[]; // This is our local copy of leds.

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

    effectState.activeEffect = (effectState.activeEffect + 1) % EFFECT_CNT;
    effectState.age = 0;
    lastDataCreationTs = 0;
    effectState.sourceTransmitterId = effectState.transmitterId; // Set it to us, this ain't a relay.
    transmitEffectState(&effectState);                           // Force a transmission loop
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
  Serial.println(effectState.transmitterId);
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

  effectState.loopPosition = (millis() + effectLoopClockOffset) % config.EFFECT_LOOP_MS;

  if (hasGottenSync)
  {
    // Button doesn't work until we get button sync
    setButtonState();
  }

  effectLoop(&effectState);

  FastLED.show();
}
