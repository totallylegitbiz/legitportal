#include <SPI.h>
#include <FastLED.h>
#include <Config.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>

//address through which two modules communicate.
const byte address[6] = "00001";

const uint16_t pingIntervalMin = 500;
const uint16_t pingIntervalMax = 1000;

uint16_t pingIntervalMs = random(pingIntervalMin, pingIntervalMax); // How often will we ping.
uint32_t pingLastPingMs = 0;                                        // Time of the last ping
uint32_t dataLastReceived = 0;
const uint16_t txFailureResetMs = 10 * 1000; // After 10 seconds of failures, reset
uint32_t lastSuccessfulTx = 0;
uint32_t lastDataCreationTs = 0;

RF24 radio(config.RADIO_CE_PIN, config.RADIO_CSN_PIN); // CE, CSN pins
EffectDataPacket nextEffectDataPacket;

// Dealing with presync
bool hasGottenSync = false;
const uint8_t syncTimeout = yesRandom(pingIntervalMs * 2, pingIntervalMs * 5); //Wait until at most double the timeout until starting to transmit.

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

void transmitterSetup()
{

  setupStatusLED();
  radioSetup();
}

void (*resetFunc)(void) = 0;

void transmitEffectDataPacket(struct EffectDataPacket *effectState)
{
  // We are broadcasting our data.

  radio.stopListening(); // Stop the radio for a hot second.

  // EffectDataPacket objEffectDataPacket(*effectState);

  EffectDataPacket objEffectDataPacket = *effectState;

  objEffectDataPacket.age = millis() - lastDataCreationTs;

  bool ok = radio.write(&objEffectDataPacket, sizeof(objEffectDataPacket));

  pingIntervalMs = yesRandom(pingIntervalMin, pingIntervalMax);

  Serial.print("Package: age=");
  Serial.println(objEffectDataPacket.age);
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
    Serial.print("INCOMING: ");
    radio.read(&nextEffectDataPacket, sizeof(nextEffectDataPacket));

    Serial.print(" txId: ");
    Serial.println(nextEffectDataPacket.transmitterId);

    if (nextEffectDataPacket.transmitterId == 0)
    {
      // Somehow we have an invalid transmitterId, ignore it.
      Serial.println("Ignoring invalid transmission ");

      return;
    }

    hasGottenSync = true; // We got a sync!
    int nextEffectLoopClockOffset = nextEffectDataPacket.loopPosition - (millis() % config.EFFECT_LOOP_MS);

    Serial.println("RX ");
    // Serial.print(nextEffectDataPacket.transmitterId);
    // Serial.print(" -> ");
    // Serial.print(effectState->transmitterId);
    // Serial.print(" loopPosition: ");
    // Serial.print(nextEffectDataPacket.loopPosition);
    // Serial.print(" activeEffect: ");
    // Serial.println(nextEffectDataPacket.activeEffect);

    bool hasEffectChanged = effectState->activeEffect != nextEffectDataPacket.activeEffect;
    bool hasSourceChanged = effectState->sourceTransmitterId != nextEffectDataPacket.sourceTransmitterId;
    bool isFromMe = nextEffectDataPacket.sourceTransmitterId == config.TRANSMITTER_ID;

    bool shouldRelay = (hasEffectChanged || hasSourceChanged);

    if (hasEffectChanged)
    {
      Serial.print("EFFECT CHANGE! from: ");
      Serial.print(effectState->activeEffect);
      Serial.print(" to ");
      Serial.println(nextEffectDataPacket.activeEffect);
    }

    if (isFromMe)
    {
      // This is from me, ignore it.
      Serial.print("Ignoring my own relayed transmssion. Diff: ");
      Serial.println(effectLoopClockOffset - nextEffectLoopClockOffset);
      return;
    }

    if (millis() - nextEffectDataPacket.age < lastDataCreationTs)
    {
      // This new data is older than mine, not applying, but relaying.
      Serial.print("Marked stale. age: ");
      Serial.print(nextEffectDataPacket.age);
      Serial.print(" lastDataCreationTs ago ");
      Serial.println(millis() - lastDataCreationTs);
      // Serial.print(" diff ");
      // Serial.println((millis() - nextEffectDataPacket.age) - lastDataCreationTs);
      // Serial.print(" diff ");
      // Serial.println((millis() - nextEffectDataPacket.age) - lastDataCreationTs);

      // if (shouldRelay)
      // {
      //     transmitEffectDataPacket(effectState);
      // }
      // return;
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

    Serial.print("EFFECT");
    Serial.println(nextEffectDataPacket.activeEffect);

    if (shouldRelay)
    {
      transmitEffectDataPacket(effectState);
    }
  }

  if (!hasGottenSync && millis() > syncTimeout)
  {
    Serial.println("Sync timeout reached, starting..");
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

  // if (!isDataFresh)
  // {
  //     Serial.println("Has stale data...");
  // }

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