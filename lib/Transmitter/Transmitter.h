#include <SPI.h>
#include <FastLED.h>
#include <Config.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>

//address through which two modules communicate.
const byte address[6] = "AC28PR";

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
const uint32_t syncTimeout = 0; //pingIntervalMax * 2; //Wait until at most double the timeout until starting to transmit.

// Temporary override.
const uint16_t OVERRIDE_TIMEOUT = 10000; // 10 seconds after device stops receiving messages.

const uint16_t MAX_AGE = 10000;
const uint16_t MAX_AGE_GRACE = 5000;

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

#if (EDEBUG + 0)
  radio.setPALevel(RF24_PA_MIN);
#else
  radio.setPALevel(RF24_PA_MAX);
#endif

  radio.setAutoAck(false);

  // Not sure what this does.
  radio.setCRCLength(RF24_CRC_8);

  // Disable dynamic payloads
  // write_register(DYNPD, 0);

  radio.disableDynamicPayloads();
  radio.setRetries(15, 15);

  radio.openReadingPipe(0, address);
  radio.openWritingPipe(address);

  radio.startListening();

  printf_begin();
  radio.printDetails();

  SERIAL_PRINTLN("READY!");
  nextEffectDataPacket.loopPosition = 0;
  nextEffectDataPacket.activeEffect = 0;
}

void transmitterSetup(struct EffectDataPacket *effectState)
{

  SERIAL_PRINTLN("### Transmitter setup: ");
  effectState->transmitterId = config.TRANSMITTER_ID; // Set it to us, this ain't a relay.
  effectState->role = config.ROLE;

  SERIAL_PRINT("@@@ ROLE:");
  SERIAL_PRINTLN((int)config.ROLE);

  setupStatusLED();
  radioSetup();

  SERIAL_PRINT("### SETUP COMPLETE for transmitter id: ");
  SERIAL_PRINTLN(config.TRANSMITTER_ID);
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
    effectState->sourceTransmitterId = effectState->transmitterId;
  }
  else
  {
    objEffectDataPacket.age = millis() - lastDataCreationTs;
  }

  bool ok = radio.write(&objEffectDataPacket, sizeof(objEffectDataPacket));

  SERIAL_PRINT("TX: ");

  SERIAL_PRINT("effectModifiter->");
  SERIAL_PRINT(objEffectDataPacket.effectModifier);
  if (ok)
  {
    SERIAL_PRINTLN(": sent");
    pingLastPingMs = millis(); // Successfully sent..
    lastSuccessfulTx = millis();
  }
  else
  {
    SERIAL_PRINTLN(": failed!!!!");
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
      SERIAL_PRINTLN("Ignoring invalid transmission...");

      return;
    }

    const bool isSameRoleAsMe = nextEffectDataPacket.role == effectState->role;

    const bool isRemote = nextEffectDataPacket.role == DeviceRole::ATARI;

    const bool isOverRide = isRemote || (!isSameRoleAsMe && nextEffectDataPacket.role == DeviceRole::CAMP);

    uint32_t nextEffectLoopClockOffset = nextEffectDataPacket.loopPosition - (millis() % config.EFFECT_LOOP_MS);

    SERIAL_PRINT("TX from role: ");
    SERIAL_PRINTLN((int)nextEffectDataPacket.role);
    SERIAL_PRINT("isSameRoleAsMe: ");
    SERIAL_PRINTLN(isSameRoleAsMe);

    if (isOverRide)
    {
      // This is a CAMP device. Camp devices override local settings for 20 seconds after the last ping.
      // Camp device data is not relayed, just set the over ride.
      SERIAL_PRINTLN("Received overide.");
      overRideUntilTs = millis() + OVERRIDE_TIMEOUT;
    }

    if (!isRemote && !isOverRide && !isSameRoleAsMe)
    {
      // I'm gonna ignore this.
      SERIAL_PRINT("Mis-matched role, ignoring: DEVICE_ROLE=");
      SERIAL_PRINTLN((int)nextEffectDataPacket.role);

      return;
    }

    bool hasEffectChanged = effectState->activeEffect != nextEffectDataPacket.activeEffect;
    bool hasSourceChanged = effectState->sourceTransmitterId != nextEffectDataPacket.sourceTransmitterId;
    bool isFromMe = nextEffectDataPacket.sourceTransmitterId == config.TRANSMITTER_ID;

    bool shouldRelay = isRemote || (!isOverRide && (hasEffectChanged || hasSourceChanged));

    if (isFromMe)
    {
      // This is from me, ignore it.
      SERIAL_PRINT("Ignoring my own relayed transmssion. Drift: ");
      SERIAL_PRINTLN(effectLoopClockOffset - nextEffectLoopClockOffset);
      return;
    }

    // If local age is less than received age, continue.
    // if local age is < recieved age it is statle.

    uint32_t localDataAge = millis() - lastDataCreationTs;

    if (!isOverRide && hasGottenSync && localDataAge < nextEffectDataPacket.age)
    {
      // This new data is older than mine, not applying, but relaying.
      // If we haven't gotten sync, skip.
      SERIAL_PRINT("Marked stale. age: ");
      SERIAL_PRINT(nextEffectDataPacket.age);
      SERIAL_PRINT(" lastDataCreationTs ago ");
      SERIAL_PRINTLN(millis() - lastDataCreationTs);

      // Marked stale. age: 0 lastDataCreationTs ago 265332
      if (shouldRelay)
      {
        transmitEffectDataPacket(effectState);
      }
      return;
    }

    if (hasEffectChanged)
    {
      SERIAL_PRINT("EFFECT CHANGE! from: ");
      SERIAL_PRINT(effectState->activeEffect);
      SERIAL_PRINT(" to ");
      SERIAL_PRINTLN(nextEffectDataPacket.activeEffect);
    }

    // We only relay if the activeEffect has changed or the souce changed;

    // Copy over the state to our local state.
    effectState->activeEffect = nextEffectDataPacket.activeEffect;
    Serial.println("nextEffectDataPacket.effectModifier ->");
    Serial.println(nextEffectDataPacket.effectModifier);

    effectState->effectModifier = nextEffectDataPacket.effectModifier;
    effectState->sourceTransmitterId = nextEffectDataPacket.sourceTransmitterId;
    effectState->age = nextEffectDataPacket.age;

    //  TODO(jorgelo): Some logic here incase the drift is too great.
    effectLoopClockOffset = nextEffectLoopClockOffset;

    lastDataCreationTs = millis() - effectState->age;
    dataLastReceived = millis();

    SERIAL_PRINT("RX: #");
    SERIAL_PRINT(nextEffectDataPacket.transmitterId);
    SERIAL_PRINT(" -> ");
    SERIAL_PRINT(" loopPosition: ");
    SERIAL_PRINT(nextEffectDataPacket.loopPosition);
    SERIAL_PRINT(" effectModifier: ");
    SERIAL_PRINT(nextEffectDataPacket.effectModifier);
    SERIAL_PRINT(" age: ");
    SERIAL_PRINT(nextEffectDataPacket.age);
    SERIAL_PRINT(" activeEffect: ");
    SERIAL_PRINTLN(nextEffectDataPacket.activeEffect);

    if (shouldRelay)
    {
      transmitEffectDataPacket(effectState);
    }

    hasGottenSync = true; // We got a sync!
  }

  if (!hasGottenSync && millis() > syncTimeout)
  {
    SERIAL_PRINTLN("Sync timeout reached, starting..");
    SERIAL_PRINT("syncTimeout: ");
    SERIAL_PRINTLN(syncTimeout);
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

  const bool isMine = effectState->sourceTransmitterId == effectState->transmitterId;

  if (hasGottenSync && shouldPing)
  {
    if (isMine)
    {
      // This is mine, ping an update with a new age.
      transmitEffectDataPacket(effectState, true);
    }
    else
    {
      // Transmit as it.
      transmitEffectDataPacket(effectState);
    }
    pingLastPingMs = millis();
  }
}

void transmitterLoop(struct EffectDataPacket *effectState)
{

  // const bool writeOnly = config.ROLE == DeviceRole::ATARI;

  if (millis() > overRideUntilTs)
  {
    // Only transmit when we don't have an active over ride.
    // if ()
    // MAX_AGE
    // MAX_AGE_GRACE

    if (effectState->age > MAX_AGE)
    {
      // Alright, this is old.
      const bool isMine = effectState->sourceTransmitterId == effectState->transmitterId;

      if (isMine || effectState->age > MAX_AGE)
      {
        // This is mine, ping an update with a new age.
        transmitEffectDataPacket(effectState, true);
      };
    }

    transmitterTransmitLoop(effectState);
  }
  // if (!writeOnly)
  // {
  // If it's atari, don't receive, doesn't mattter.
  transmitterReceiveLoop(effectState);
  // }
}