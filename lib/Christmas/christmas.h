#include <Arduino.h>
#include <FastLED.h>
#include <math.h>
#include <config.h>
#include <util.h>
#include <candle.h>

void setupChristmas()
{
}

void loopChristmas(CRGB cleds[LED_NUM], InputState inputState)
{
    // int hue = inputState.PADDLE_VALUE[0] % 255;
    // int sat = float(inputState.PADDLE_VALUE[0]) / 1024 * 255;

    // float loopPercent = float(inputState.MS_ELAPSED % LOOP_MS) / LOOP_MS;

    // Serial.print(loopPercent);
    // Serial.print("\n");

    // int selectedLED = LED_NUM * loopPercent;

    // Serial.print(selectedLED);
    // Serial.print("\n");

    // int LOOP_MS = 1000 * 10;
    // randomSeed(inputState.MS_ELAPSED);

    // 175hrs

    int SECOND = 1000;
    int HRTZ = 175;
    int LEN_SEC = CANDLE_LEN / 175;

    for (int i = 0; i < LED_NUM; i++)
    {
        int LOOP_WIDTH = 10;
        int LOOP_OFFSET = (i % LOOP_WIDTH) - (LOOP_WIDTH / 2);
        float LOOP_PERCENT = (LOOP_OFFSET * LOOP_OFFSET / LOOP_WIDTH); // * (random(0,100)/100.00)*2.0;

        //  float loopPercent = float(LOOP_OFFSET) / LED_NUM;

        // CHSV(23, 255, 50);

        // cleds[i] = CRGB(220, 92, 12);

        // float hueMutl = 1 + (LOOP_PERCENT / 5.0) - .5;
        // float satMult = 1;
        // // Math.sin(*Math.PI/2)
        // // float intMult = 1 + ((LOOP_PERCENT / .5));
        // float intMult = sin(0.5 * PI);

        //

        // cos(PI * LOOP_PERCENT);     //1 + ((LOOP_PERCENT / 1.0)*.5);

        // Serial.print(LOOP_OFFSET);
        // Serial.print("\n");
        // delay(10);
        // cleds[i] = CHSV(23 * hueMutl, 255 * satMult, 100 * intMult); //CHSV(0, 255, 40); //CHSV(23, 255, 50); //CRGB{215, 92, 12};C
        // CANDLE[0];
        // CANDLE_LEN;

        cleds[i] = CHSV(23, 255, 80);
    }
    // cleds[selectedLED] = CHSV(23, 255, 100); //CHSV(23, 255, 50); //CRGB{215, 92, 12};
}

// Yellows
// rgb(252,249,142)
// rgb(249,168,68)
// rgb(215,92,12)