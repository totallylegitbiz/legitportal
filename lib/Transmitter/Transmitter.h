#include <SPI.h>
#include <FastLED.h>
#include <Config.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>

//address through which two modules communicate.
const byte address[6] = "00001";

int pingIntervalMs = 3000;            // How often will we ping.
bool pingLastOffset = pingIntervalMs; // Time since last ping;

RF24 radio(RADIO_CE_PIN, RADIO_CSN_PIN); // CE, CSN pins
EffectState nextEffectState;

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

void transmitterSetup()
{
    setupStatusLED();
    radioSetup();
}

// unsigned int effectLoopClockOffset = 0;

void transmitterLoop(struct EffectState *effectState)
{
    const int pingOffset = millis() % pingIntervalMs;

    if (pingOffset < pingLastOffset)
    {

        Serial.print("WE BE BROADCASTING");
        // We are broadcasting our data.

        pingIntervalMs = random(1 * 1000, 3 * 1000); // Let's randomize it.

        radio.stopListening(); // Stop the radio for a hot second.
        blink(GREEN_LED_PIN);

        EffectState objEffectState(*effectState);

        // bool ok = radio.write(&effectState, sizeof(effectState));
        bool ok = radio.write(&objEffectState, sizeof(objEffectState));

        Serial.print("Sending: ");
        Serial.println(objEffectState.loopPosition);
        if (ok)
        {
            Serial.println("sent.");
            // Serial.println(effectState);
            // blink(BLUE_LED_PIN);
        }
        else
        {
            Serial.println("failed...");
            // blink(RED_LED_PIN);
        }

        radio.startListening();

        // blink(GREEN_LED_PIN);
    }

    if (radio.available())
    {

        radio.read(&nextEffectState, sizeof(nextEffectState));
        Serial.print("nextEffectState.:loopPosition:");
        Serial.println(nextEffectState.loopPosition);

        int nextEffectLoopClockOffset = nextEffectState.loopPosition - (millis() % EFFECT_LOOP_MS);

        // //  TODO(jorgelo): Some logic here incase the drift is too great.
        effectLoopClockOffset = nextEffectLoopClockOffset;

        // Serial.print("effectLoopClockOffset: ");
        // Serial.println(effectLoopClockOffset);

        // blink(GREEN_LED_PIN);
    }
}
