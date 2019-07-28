#include <SPI.h>
#include <FastLED.h>
#include <Config.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>

//address through which two modules communicate.
const byte address[6] = "00001";

const unsigned int pingIntervalMs = random(500, 1500); // How often will we ping.
unsigned long pingLastPingMs = 0;                      // Time of the last ping
unsigned long dataLastReceived = 0;

RF24 radio(RADIO_CE_PIN, RADIO_CSN_PIN); // CE, CSN pins
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
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(BLUE_LED_PIN, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    blink(RED_LED_PIN);
    blink(GREEN_LED_PIN);
    blink(BLUE_LED_PIN);
}

void radioSetup()
{

    radio.begin();
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_MIN);
    radio.setAutoAck(false);
    // radio.setAutoAck(true);
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
    currentEffectState.transmitterId = random(0, 255);
}

void transmitterSetup()
{

    // Setup Effect State
    currentEffectState.activeEffect = 255; // Special loading state
    transmitterSetId();

    setupStatusLED();
    radioSetup();
}

void transmitEffectState(struct EffectState *effectState)
{
    // We are broadcasting our data.

    // pingIntervalMs = random(1 * 1000, 3 * 1000); // Let's randomize it.

    radio.stopListening(); // Stop the radio for a hot second.
    // blink(GREEN_LED_PIN);

    EffectState objEffectState(*effectState);

    // bool ok = radio.write(&effectState, sizeof(effectState));
    bool ok = radio.write(&objEffectState, sizeof(objEffectState));

    Serial.print(effectState->transmitterId);
    Serial.print(" -> * loopPosition");
    Serial.print(objEffectState.loopPosition);
    Serial.print(" activeEffect: ");
    Serial.print(objEffectState.activeEffect);

    if (ok)
    {
        Serial.println(": sent");
        // Serial.println(effectState);
        // blink(BLUE_LED_PIN);
    }
    else
    {
        Serial.println("; failed");
        // blink(RED_LED_PIN);
    }

    radio.startListening();

    pingLastPingMs = millis();
    // blink(GREEN_LED_PIN);
}

void transmitterReceiveLoop(struct EffectState *effectState)
{

    if (radio.available())
    {
        hasGottenSync = true; // We got a sync!
        radio.read(&nextEffectState, sizeof(nextEffectState));

        int nextEffectLoopClockOffset = nextEffectState.loopPosition - (millis() % EFFECT_LOOP_MS);

        Serial.print(nextEffectState.transmitterId);
        Serial.print(" -> ");
        Serial.print(effectState->transmitterId);
        Serial.print(" loopPosition: ");
        Serial.print(nextEffectState.loopPosition);
        Serial.print(" activeEffect: ");
        Serial.println(nextEffectState.activeEffect);

        // recievedStatusEffect();

        if (nextEffectState.sourceTransmitterId == effectState->transmitterId)
        {
            // This is from me, ignore it.
            Serial.print("Ignoring my own relayed transmssion. Diff: ");
            Serial.println(nextEffectLoopClockOffset - effectLoopClockOffset);
            return;
        }

        // We only relay if the activeEffect has changed or the souce changed;

        bool shouldRelay = effectState->activeEffect != nextEffectState.activeEffect || effectState->sourceTransmitterId != nextEffectState.sourceTransmitterId;

        // Copy over the state to our local state.
        effectState->activeEffect = nextEffectState.activeEffect;
        effectState->sourceTransmitterId = nextEffectState.sourceTransmitterId;

        // We gotta relay this only if it differs from our ac

        if (shouldRelay)
        {
            transmitEffectState(effectState);
        }

        // //  TODO(jorgelo): Some logic here incase the drift is too great.
        effectLoopClockOffset = nextEffectLoopClockOffset;

        // Serial.print("effectLoopClockOffset: ");
        // Serial.println(effectLoopClockOffset);

        // blink(GREEN_LED_PIN);

        dataLastReceived = millis();
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

    const bool isDataFresh = millis() > dataLastReceived + pingIntervalMs;
    const bool withinPingInterval = millis() > (pingIntervalMs + pingLastPingMs);
    const bool shouldPing = withinPingInterval && isDataFresh;

    if (withinPingInterval && !isDataFresh)
    {
        Serial.println("data is stale.");
    }

    // Serial.print("Should ping? ");
    // Serial.println(shouldPing);
    // Serial.print("millis() ");
    // Serial.println(millis());
    // Serial.print("pingLastPingMs");
    // Serial.println(pingLastPingMs);
    // Serial.print("pingIntervalMs");
    // Serial.println(pingIntervalMs);

    if (hasGottenSync && shouldPing)
    {
        transmitEffectState(effectState);
        pingLastPingMs = millis();
    }
}
