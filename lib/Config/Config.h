#ifndef INC_CONFIG
#define INC_CONFIG

// Pins for RGB status leds
// const int RED_LED_PIN = 2;
// const int GREEN_LED_PIN = 3;
// const int BLUE_LED_PIN = 4;
const boolean DIAGNOSTIC_MODE = true;

const int RED_LED_PIN = A0;
const int GREEN_LED_PIN = A1;
const int BLUE_LED_PIN = A2;

const int DIP_PIN_0 = 2;
const int DIP_PIN_1 = 3;
const int DIP_PIN_2 = 4;
const int DIP_PIN_3 = 5;

// Pulse
const int PULSE_SENSOR_PIN = A7;

// // LED strip control pin
const int LED_PIN = 8;
const uint8_t LED_BRIGHTNESS = 50; // 0-255 for overall brightness.

// Led strip
const unsigned int LED_CNT = 98;
const unsigned int LED_OFFSET = 0; //25;

// RADIO
const int RADIO_CE_PIN = 9;
const int RADIO_CSN_PIN = 10;

// EFFECT BUTTON
const int EFFECT_BUTTON_PIN = 6;

// Effect Loop config
const unsigned int EFFECT_LOOP_MS = 60 * 1000;
#endif

/*

D12 -> RF24 MISO 
D11 -> RF24 MOSI
D10 - RF24 CSN
D9 -> RF24 CE
D8 -
D6 - 
D4 - 
D3 - 
D2 - 
GND -
RESET - 
D0/RXD - 
D1/TXD - 

D13 -> RF24 SCK
3v3 -> POWER SELECTOR
REF
A0 - RBG RED  
A1 - RGB Green
A2 - RGB BLUE
A3 - J
A4 - 
A5 - 
A6 -
A7 -
5V - 
GND -
VIN - 
 */