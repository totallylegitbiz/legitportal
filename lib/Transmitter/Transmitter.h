#include <SPI.h>
#include <FastLED.h>
#include <Config.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>

//address through which two modules communicate.
const byte address[6] = "00001";

const uint16_t pingIntervalMin = 1000;
const uint16_t pingIntervalMax = 5000;

uint32_t pingIntervalMs = random(pingIntervalMin, pingIntervalMax); // How often will we ping.
uint32_t pingLastPingMs = 0;                                        // Time of the last ping
uint32_t dataLastReceived = 0;
const uint16_t txFailureResetMs = 10 * 1000; // After 10 seconds of failures, reset
uint32_t lastSuccessfulTx = 0;
uint32_t lastDataCreationTs = 0;

RF24 radio(config.RADIO_CE_PIN, config.RADIO_CSN_PIN); // CE, CSN pins
EffectDataPacket nextEffectDataPacket;

// Dealing with presync
bool hasGottenSync = false;
const uint32_t syncTimeout = pingIntervalMax * 2; //Wait until at most double the timeout until starting to transmit.

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
  nextEffectDataPacket.loopPosition = 0;
  nextEffectDataPacket.activeEffect = 0;
}

void transmitterSetup(struct EffectDataPacket *effectState)
{

  effectState->transmitterId = config.TRANSMITTER_ID; // Set it to us, this ain't a relay.

  setupStatusLED();
  radioSetup();
}

void (*resetFunc)(void) = 0;

void transmitEffectDataPacket(struct EffectDataPacket *effectState, bool force = false)
{
  // We are broadcasting our data.

  radio.stopListening(); // Stop the radio for a hot second.

  EffectDataPacket objEffectDataPacket = *effectState;

  if (force)
  {
    objEffectDataPacket.age = 0;
    effectState->age = 0;
    lastDataCreationTs = millis();
  }
  else
  {
    objEffectDataPacket.age = millis() - lastDataCreationTs;
  }

  bool ok = radio.write(&objEffectDataPacket, sizeof(objEffectDataPacket));

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

  pingIntervalMs = yesRandom(pingIntervalMin, pingIntervalMax);
}

void transmitterReceiveLoop(struct EffectDataPacket *effectState)
{

  if (radio.available())
  {

    radio.read(&nextEffectDataPacket, sizeof(nextEffectDataPacket));

    if (nextEffectDataPacket.transmitterId == 0)
    {
      // Somehow we have an invalid transmitterId, ignore it.
      Serial.println("Ignoring invalid transmission...");

      return;
    }

    uint32_t nextEffectLoopClockOffset = nextEffectDataPacket.loopPosition - (millis() % config.EFFECT_LOOP_MS);

    bool hasEffectChanged = effectState->activeEffect != nextEffectDataPacket.activeEffect;
    bool hasSourceChanged = effectState->sourceTransmitterId != nextEffectDataPacket.sourceTransmitterId;
    bool isFromMe = nextEffectDataPacket.sourceTransmitterId == config.TRANSMITTER_ID;

    bool shouldRelay = (hasEffectChanged || hasSourceChanged);

    if (isFromMe)
    {
      // This is from me, ignore it.
      Serial.print("Ignoring my own relayed transmssion. Drift: ");
      Serial.println(effectLoopClockOffset - nextEffectLoopClockOffset);
      return;
    }

    // If local age is less than received age, continue.
    // if local age is < recieved age it is statle.

    uint16_t localDataAge = millis() - lastDataCreationTs;

    if (hasGottenSync && localDataAge < nextEffectDataPacket.age)
    {
      // This new data is older than mine, not applying, but relaying.
      // If we haven't gotten sync, skip.
      Serial.print("Marked stale. age: ");
      Serial.print(nextEffectDataPacket.age);
      Serial.print(" lastDataCreationTs ago ");
      Serial.println(millis() - lastDataCreationTs);

      // Marked stale. age: 0 lastDataCreationTs ago 265332
      if (shouldRelay)
      {
        transmitEffectDataPacket(effectState);
      }
      return;
    }

    if (hasEffectChanged)
    {
      Serial.print("EFFECT CHANGE! from: ");
      Serial.print(effectState->activeEffect);
      Serial.print(" to ");
      Serial.println(nextEffectDataPacket.activeEffect);
    }

    // We only relay if the activeEffect has changed or the souce changed;

    // Copy over the state to our local state.
    effectState->activeEffect = nextEffectDataPacket.activeEffect;
    effectState->sourceTransmitterId = nextEffectDataPacket.sourceTransmitterId;
    effectState->age = nextEffectDataPacket.age;

    //  TODO(jorgelo): Some logic here incase the drift is too great.
    effectLoopClockOffset = nextEffectLoopClockOffset;

    lastDataCreationTs = millis() - effectState->age;
    dataLastReceived = millis();

    Serial.print("RX: #");
    Serial.print(nextEffectDataPacket.transmitterId);
    Serial.print(" -> ");
    Serial.print(" loopPosition: ");
    Serial.print(nextEffectDataPacket.loopPosition);
    Serial.print(" age: ");
    Serial.print(nextEffectDataPacket.age);
    Serial.print(" activeEffect: ");
    Serial.println(nextEffectDataPacket.activeEffect);

    if (shouldRelay)
    {
      transmitEffectDataPacket(effectState);
    }

    hasGottenSync = true; // We got a sync!
  }

  if (!hasGottenSync && millis() > syncTimeout)
  {
    Serial.println("Sync timeout reached, starting..");
    Serial.print("syncTimeout: ");
    Serial.println(syncTimeout);
    hasGottenSync = true;
    effectState->activeEffect = 0;
    effectState->sourceTransmitterId = config.TRANSMITTER_ID; // Since we are assuming our own effect, we are the source now.
    transmitEffectDataPacket(effectState);
    pingLastPingMs = millis(); // reset the ping time.
    dataLastReceived = millis();
  }
}

void transmitterTransmitLoop(struct EffectDataPacket *effectState)
{

  const uint16_t dataGracePeriod = pingIntervalMs * 3;

  const bool isWithinGracePeriod = millis() < dataLastReceived + dataGracePeriod;
  const bool isDataFresh = millis() < (dataLastReceived + pingIntervalMax);
  const bool isWithinPingInterval = millis() > (pingIntervalMs + pingLastPingMs);

  const bool shouldPing = isWithinPingInterval && (isDataFresh || !isWithinGracePeriod);

  if (hasGottenSync && shouldPing)
  {
    transmitEffectDataPacket(effectState);
    pingLastPingMs = millis();
  }
}

void transmitterLoop(struct EffectDataPacket *effectState)
{
  transmitterTransmitLoop(effectState);
  transmitterReceiveLoop(effectState);
}