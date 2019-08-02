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
    currentEffectState.transmitterId = random(1, 65535);
}

void transmitterSetup()
{

    // Setup Effect State
    currentEffectState.activeEffect = 255; // Special loading state
    transmitterSetId();

    setupStatusLED();
    radioSetup();
}
void (*resetFunc)(void) = 0;

void transmitEffectState(struct EffectState *effectState)
{
    // We are broadcasting our data.

    radio.stopListening(); // Stop the radio for a hot second.
    // blink(GREEN_LED_PIN);

    EffectState objEffectState(*effectState);

    // bool ok = radio.write(&effectState, sizeof(effectState));
    bool ok = radio.write(&objEffectState, sizeof(objEffectState));

    Serial.print("TX ");
    Serial.print(effectState->transmitterId);
    Serial.print(" -> * loopPosition: ");
    Serial.print(objEffectState.loopPosition);
    Serial.print(" activeEffect: ");
    Serial.print(objEffectState.activeEffect);

    pingIntervalMs = random(pingIntervalMin, pingIntervalMax);

    if (ok)
    {
        Serial.println(": sent");
        // Serial.println(effectState);
        // blink(BLUE_LED_PIN);
        pingLastPingMs = millis(); // Successfully sent..
        lastSuccessfulTx = millis();
    }
    else
    {
        Serial.println(": failed!!!!");
        recievedStatusEffect(BlackLightFluorescent, 250);
        pingLastPingMs = millis() + (pingIntervalMs * 2); // Delay for another two intervals;
        // blink(RED_LED_PIN);

        if (millis() > lastSuccessfulTx + txFailureResetMs)
        {
            resetFunc();
        }
    }

    radio.startListening();

    pingIntervalMs = random(pingIntervalMin, pingIntervalMax);

    // blink(GREEN_LED_PIN);
}

void transmitterReceiveLoop(struct EffectState *effectState)
{

    if (radio.available())
    {

        radio.read(&nextEffectState, sizeof(nextEffectState));

        if (nextEffectState.transmitterId == 0)
        {
            // Somehow we have an invalid transmitterId, ignore it.
            Serial.println("Ignoring invalid transmission ");

            return;
        }

        hasGottenSync = true; // We got a sync!
        int nextEffectLoopClockOffset = nextEffectState.loopPosition - (millis() % EFFECT_LOOP_MS);

        Serial.print("RX ");
        Serial.print(nextEffectState.transmitterId);
        Serial.print(" -> ");
        Serial.print(effectState->transmitterId);
        Serial.print(" loopPosition: ");
        Serial.print(nextEffectState.loopPosition);
        Serial.print(" activeEffect: ");
        Serial.println(nextEffectState.activeEffect);

        if (nextEffectState.sourceTransmitterId == effectState->transmitterId)
        {
            // This is from me, ignore it.
            Serial.print("Ignoring my own relayed transmssion. Diff: ");
            Serial.println(effectLoopClockOffset - nextEffectLoopClockOffset);
            return;
        }

        // We only relay if the activeEffect has changed or the souce changed;
        bool shouldRelay = effectState->shouldRelay && (effectState->activeEffect != nextEffectState.activeEffect || effectState->sourceTransmitterId != nextEffectState.sourceTransmitterId);

        // Copy over the state to our local state.
        effectState->activeEffect = nextEffectState.activeEffect;
        effectState->sourceTransmitterId = nextEffectState.sourceTransmitterId;

        //  TODO(jorgelo): Some logic here incase the drift is too great.
        effectLoopClockOffset = nextEffectLoopClockOffset;

        dataLastReceived = millis();

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
