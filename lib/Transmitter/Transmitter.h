#include <SPI.h>
#include <FastLED.h>
#include <Config.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>

//address through which two modules communicate.
const byte address[6] = "00001";

const unsigned int pingIntervalMin = 500;
const unsigned int pingIntervalMax = 1000;
unsigned int pingIntervalMs = random(pingIntervalMin, pingIntervalMax); // How often will we ping.
unsigned long pingLastPingMs = 0;                                       // Time of the last ping
unsigned long dataLastReceived = 0;
const unsigned int txFailureResetMs = 10 * 1000; // After 10 seconds of failures, reset
unsigned long lastSuccessfulTx = 0;
unsigned long lastDataCreationTs = 0;

RF24 radio(config.RADIO_CE_PIN, config.RADIO_CSN_PIN); // CE, CSN pins
EffectState nextEffectState;

// Dealing with presync
bool hasGottenSync = false;
const unsigned int syncTimeout = random(pingIntervalMs * 2, pingIntervalMs * 5); //Wait until at most double the timeout until starting to transmit.

void blink(int pin)
{
  digitalWrite(pin, HIGH);
  delay(50);
  digitalWrite(pin, LOW);
}

void setupStatusLED()
{
  pinMode(config.RED_LED_PIN, OUTPUT);
  pinMode(config.GREEN_LED_PIN, OUTPUT);
  pinMode(config.BLUE_LED_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  blink(config.RED_LED_PIN);
  blink(config.GREEN_LED_PIN);
  blink(config.BLUE_LED_PIN);
}

void radioSetup()
{

  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN);
  radio.setAutoAck(false);

  radio.disableDynamicPayloads();
  radio.setRetries(15, 15);

  radio.openReadingPipe(0, address);
  radio.openWritingPipe(address);

  radio.startListening();

  printf_begin();
  radio.printDetails();

  Serial.println("READY!");
  randomSeed(analogRead(0));
  nextEffectState.loopPosition = 0;
  nextEffectState.activeEffect = 0;
}

void transmitterSetId()
{
  config.TRANSMITTER_ID = random(1, 65535);
}

void transmitterSetup()
{

  // Setup Effect State
  // currentEffectState.activeEffect = 255; // Special loading state
  transmitterSetId();

  setupStatusLED();
  radioSetup();
}
void (*resetFunc)(void) = 0;

void transmitEffectState(struct EffectState *effectState)
{
  // We are broadcasting our data.

  radio.stopListening(); // Stop the radio for a hot second.

  // EffectState objEffectState(*effectState);

  EffectState objEffectState = *effectState;

  objEffectState.age = millis() - lastDataCreationTs;

  bool ok = radio.write(&objEffectState, sizeof(objEffectState));

  // Serial.print("TX ");
  // Serial.print(effectState->transmitterId);
  // Serial.print(" -> * loopPosition: ");
  // Serial.print(objEffectState.loopPosition);
  // Serial.print(" activeEffect: ");
  // Serial.print(objEffectState.activeEffect);
  // Serial.print(" age: ");
  // Serial.print(objEffectState.age);

  pingIntervalMs = random(pingIntervalMin, pingIntervalMax);

  if (ok)
  {
    Serial.println(": sent");
    pingLastPingMs = millis(); // Successfully sent..
    lastSuccessfulTx = millis();
  }
  else
  {
    Serial.println(": failed!!!!");
    // recievedStatusEffect(BlackLightFluorescent, 250);
    pingLastPingMs = millis() + (pingIntervalMs * 2); // Delay for another two intervals;
    if (millis() > lastSuccessfulTx + txFailureResetMs)
    {
      resetFunc();
    }
  }

  radio.startListening();

  pingIntervalMs = random(pingIntervalMin, pingIntervalMax);
}

void transmitterReceiveLoop(struct EffectState *effectState)
{

  if (radio.available())
  {
    Serial.println("INCOMING");
    radio.read(&nextEffectState, sizeof(nextEffectState));

    if (nextEffectState.transmitterId == 0)
    {
      // Somehow we have an invalid transmitterId, ignore it.
      Serial.println("Ignoring invalid transmission ");

      return;
    }

    hasGottenSync = true; // We got a sync!
    int nextEffectLoopClockOffset = nextEffectState.loopPosition - (millis() % config.EFFECT_LOOP_MS);

    Serial.println("RX ");
    // Serial.print(nextEffectState.transmitterId);
    // Serial.print(" -> ");
    // Serial.print(effectState->transmitterId);
    // Serial.print(" loopPosition: ");
    // Serial.print(nextEffectState.loopPosition);
    // Serial.print(" activeEffect: ");
    // Serial.println(nextEffectState.activeEffect);

    bool hasEffectChanged = effectState->activeEffect != nextEffectState.activeEffect;
    bool hasSourceChanged = effectState->sourceTransmitterId != nextEffectState.sourceTransmitterId;
    bool isFromMe = nextEffectState.sourceTransmitterId == effectState->transmitterId;

    bool shouldRelay = (hasEffectChanged || hasSourceChanged);

    if (hasEffectChanged)
    {
      Serial.print("EFFECT CHANGE! from: ");
      Serial.print(effectState->activeEffect);
      Serial.print(" to ");
      Serial.println(nextEffectState.activeEffect);
    }

    if (isFromMe)
    {
      // This is from me, ignore it.
      Serial.print("Ignoring my own relayed transmssion. Diff: ");
      Serial.println(effectLoopClockOffset - nextEffectLoopClockOffset);
      return;
    }

    if (millis() - nextEffectState.age < lastDataCreationTs)
    {
      // This new data is older than mine, not applying, but relaying.
      Serial.print("Marked stale. age: ");
      Serial.print(nextEffectState.age);
      Serial.print(" lastDataCreationTs ago ");
      Serial.println(millis() - lastDataCreationTs);
      // Serial.print(" diff ");
      // Serial.println((millis() - nextEffectState.age) - lastDataCreationTs);
      // Serial.print(" diff ");
      // Serial.println((millis() - nextEffectState.age) - lastDataCreationTs);

      // if (shouldRelay)
      // {
      //     transmitEffectState(effectState);
      // }
      // return;
    }

    // We only relay if the activeEffect has changed or the souce changed;

    // Copy over the state to our local state.
    effectState->activeEffect = nextEffectState.activeEffect;
    effectState->sourceTransmitterId = nextEffectState.sourceTransmitterId;
    effectState->age = nextEffectState.age;

    //  TODO(jorgelo): Some logic here incase the drift is too great.
    effectLoopClockOffset = nextEffectLoopClockOffset;

    lastDataCreationTs = millis() - effectState->age;
    dataLastReceived = millis();

    Serial.print("EFFECT");
    Serial.println(nextEffectState.activeEffect);

    if (shouldRelay)
    {
      transmitEffectState(effectState);
    }
  }

  if (!hasGottenSync && millis() > syncTimeout)
  {
    Serial.println("Sync timeout reached, starting..");
    hasGottenSync = true;
    effectState->activeEffect = 0;
    effectState->sourceTransmitterId = effectState->transmitterId; // Since we are assuming our own effect, we are the source now.
    transmitEffectState(effectState);
    pingLastPingMs = millis(); // reset the ping time.
    dataLastReceived = millis();
  }
}

void transmitterTransmitLoop(struct EffectState *effectState)
{

  const unsigned int dataGracePeriod = pingIntervalMs * 3;

  const bool isWithinGracePeriod = millis() < dataLastReceived + dataGracePeriod;
  const bool isDataFresh = millis() < (dataLastReceived + pingIntervalMax);
  const bool isWithinPingInterval = millis() > (pingIntervalMs + pingLastPingMs);

  const bool shouldPing = isWithinPingInterval && (isDataFresh || !isWithinGracePeriod);

  // if (!isDataFresh)
  // {
  //     Serial.println("Has stale data...");
  // }

  if (hasGottenSync && shouldPing)
  {
    transmitEffectState(effectState);
    pingLastPingMs = millis();
  }
}
