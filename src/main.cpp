#include <SPI.h>
#include <FastLED.h>
#include <Config.h>

EffectDataPacket effectState;
const Config config = getConfig();

#include <LEDStrip.h>
#include <Effects.h>
#include <Transmitter.h>

bool lastButtonState = false; // Default is not pressed.
int lastRecievedOffset = 0;
bool isPoweredOn = true;

uint32_t buttonPressedSince = 0;

void setButtonState()
{

  const uint8_t buttonLongPressMs = 500;

  // Button Logic
  boolean currentButtonState = !digitalRead(config.EFFECT_BUTTON_PIN);

  if (currentButtonState && !lastButtonState)
  {
    // Changed state
    buttonPressedSince = millis();

    if (!isPoweredOn)
    {
      // Handle when device is powered off, to reset it.
      Serial.println("Powering up...");
      resetFunc();
    }
  }

  // Shuts down the controller.
  if (isPoweredOn && currentButtonState && (millis() - buttonPressedSince) > buttonLongPressMs)
  {
    Serial.println("Powering down...");
    isPoweredOn = false;
    FastLED.clear();
    FastLED.show();
    radio.stopListening();
    return;
  }

  if (isPoweredOn && lastButtonState && currentButtonState != lastButtonState)
  {
    // BUTTON WAS CLICKED
    Serial.println("Click");

    effectState.activeEffect = (effectState.activeEffect + 1) % EFFECT_CNT;
    effectState.sourceTransmitterId = config.TRANSMITTER_ID; // Set it to us, this ain't a relay.
    effectState.transmitterId = config.TRANSMITTER_ID;       // Set it to us, this ain't a relay.
    transmitEffectDataPacket(&effectState, true);            // Force a transmission loop
  }

  lastButtonState = currentButtonState;
}

void setup()
{

  // Setup config.

  Serial.begin(9600);

  effectSetup();

  pinMode(config.EFFECT_BUTTON_PIN, INPUT);
  digitalWrite(config.EFFECT_BUTTON_PIN, HIGH);

  pinMode(config.RED_LED_PIN, OUTPUT);
  pinMode(config.BLUE_LED_PIN, OUTPUT);
  pinMode(config.GREEN_LED_PIN, OUTPUT);

  pinMode(config.SENSOR_PIN, INPUT);

  ledStripSetup();

  transmitterSetup(&effectState);

  Serial.print("### SETUP COMPLETE for transmitter id: ");
  Serial.println(config.TRANSMITTER_ID);
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

  if (!isPoweredOn)
  {
    return;
  }

  effectLoop(&effectState);

  transmitterLoop(&effectState);
}
