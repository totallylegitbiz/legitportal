#include <SPI.h>
#include <FastLED.h>
#include <Config.h>

EffectDataPacket effectState;
const Config config = getConfig();

bool lastButtonState = false; // Default is not pressed.
int lastRecievedOffset = 0;
bool isPoweredOn = true;

// Temporary override settings;
uint32_t overRideUntilTs = 0;

// How long the button has been held since ts.
uint32_t buttonPressedSinceTs = 0;

#include <LEDStrip.h>
#include <Effects.h>
#include <Transmitter.h>

void setButtonState()
{

  const uint16_t buttonLongPressMs = 2000;

  // Button Logic
  boolean currentButtonState = !digitalRead(config.EFFECT_BUTTON_PIN);

  if (currentButtonState && !lastButtonState)
  {
    // Changed state
    buttonPressedSinceTs = millis();

    if (!isPoweredOn)
    {
      // Handle when device is powered off, to reset it.
      SERIAL_PRINTLN("Powering up...");
      resetFunc();
    }
  }

  // Shuts down the controller.
  if (isPoweredOn && currentButtonState && (millis() - buttonPressedSinceTs) > buttonLongPressMs)
  {
    SERIAL_PRINTLN("Powering down...");
    isPoweredOn = false;
    FastLED.clear();
    FastLED.show();
    radio.stopListening();
    return;
  }

  if (isPoweredOn && lastButtonState && currentButtonState != lastButtonState)
  {
    // BUTTON WAS CLICKED
    SERIAL_PRINTLN("Click");

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

  SERIAL_BEGIN(9600);

  effectSetup();

  pinMode(config.EFFECT_BUTTON_PIN, INPUT);
  digitalWrite(config.EFFECT_BUTTON_PIN, HIGH);

  pinMode(config.RED_LED_PIN, OUTPUT);
  pinMode(config.BLUE_LED_PIN, OUTPUT);
  pinMode(config.GREEN_LED_PIN, OUTPUT);

  pinMode(config.SENSOR_PIN, INPUT);

  transmitterSetup(&effectState);

  if (LED_CNT > 0)
  {
    ledStripSetup();
  }

  SERIAL_PRINTLN("### Device setup complete.");
}

int prevMemory = 0;

void loop()
{

  const int mem = freeMemory();

  if (prevMemory != mem)
  {
    SERIAL_PRINT("freeMemory()=");
    SERIAL_PRINTLN(mem);
    prevMemory = mem;
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

  if (LED_CNT > 0)
  {
    effectLoop(&effectState);
  }

  transmitterLoop(&effectState);
}
