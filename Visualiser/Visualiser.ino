#include <FastLED.h>
#include <SPI.h>
#include <Ramp.h>
#include <LibPrintf.h>
#include <lcdgfx.h>
#include <lcdgfx_gui.h>
#include <NoDelay.h>

#include "libraries/midi_config.h"
#include "Animation.h"
#include "Silkscreen.h"

static ramp animationRamp;

extern CRGBPalette16 currentPalette;
extern TBlendType currentBlending;
extern CRGB leds[];

#define BUFFER_SIZE 64
uint8_t buffer[BUFFER_SIZE];


// Speed that the LEDs animate their brightness when first power on
#define ON_RAMP_SPEED 1000

DisplaySSD1306_128x32_I2C display(-1);

void prepareSpiSlave() {
  /*
  pinMode(SS, INPUT_PULLUP);
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, INPUT);
  pinMode(MISO, OUTPUT);
*/
  // Turn on SPI in slave mode
  SPCR |= _BV(SPE);
  pinMode(MISO, OUTPUT);

  SPI.attachInterrupt();
}

void setup() {

  Serial.begin(115200);

  prepareSpiSlave();

  display.begin();
  display.clear();

  animationRamp.go(BRIGHTNESS, ON_RAMP_SPEED, CUBIC_IN, ONCEFORWARD);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(0);

  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;

  //  display.setFixedFont(ssd1306xled_font6x8/*ssd1306xled_font6x8*/);

  // Pretty good - possibly a bit too small?
  display.setFreeFont(free_slkscr10x14 /*ssd1306xled_font6x8*/);
  // display.setFreeFont(free_slkscr11x15/*ssd1306xled_font6x8*/);

  // too big/ugly
  // display.setFreeFont(free_slkscr13x19/*ssd1306xled_font6x8*/);
}

// Marker bytes
#define DATA_SEND_START 0xF1
#define DATA_SEND_END 0xE1

uint8_t bufferPos;
volatile bool packetReady = false;

// SPI interrupt routine
ISR(SPI_STC_vect) {

  // Read SPI data
  byte data = SPDR;

  // Reset buffer index at the start of a packet. Subsequent packets will override the existing packet.
  if (data == DATA_SEND_START) {
    bufferPos = 0;
    return;
  }

  if (data == DATA_SEND_END) {

    // Set flag as soon as the first packet is ready. We don't need to reset this field as subsequent
    // packets override existing values in the buffer. This is just to ensure that at least one complete
    // packet is present in the buffer.

    packetReady = true;
  }

  // add payload to buffer if room
  if (bufferPos < sizeof buffer) {
    buffer[bufferPos++] = data;
  }
}

noDelay animationDelay = noDelay(1000 / UPDATES_PER_SECOND);


void loop() {
/*
  if (animationDelay.update()) {
    animate();
  }
*/
  processInput();
}

uint8_t encoderPos;

void animate() {

  int brightness = animationRamp.update();
  FastLED.setBrightness(brightness);

  FillLEDsFromPaletteColors(encoderPos * 2);
  FastLED.show();
}

NanoCanvas<128, 10, 1> canvas;

void processInput() {

  // Nothing to do if we don't have a full packet
  if (!packetReady) return;

  char displayBuffer[256];

  uint8_t i = 0;

  do {

    // Break if we have read all encoders
    if (buffer[i] == DATA_SEND_END) break;

    uint8_t encoderIndex = buffer[i++];
    uint8_t position = buffer[i++];

Serial.print(encoderIndex);
Serial.print(" ");
Serial.println(position);

    // TEMP
    encoderPos = position;

    uint8_t pressed = buffer[i++];
    uint8_t midiControlNumber = buffer[i++];

    //    sprintf(displayBuffer, "Position: %d   ", position);
    //display.printFixed(0, 20, displayBuffer);

    // sprintf(displayBuffer, "Pressed: %d   ", pressed);
    // display.printFixed(0, 25, displayBuffer);

    switch (midiControlNumber) {
      case CC_ANY_DELAY_FB:
        sprintf(displayBuffer, "DELAY FEEDBCK");
        break;
      case CC_808_VOLUME:
        sprintf(displayBuffer, "808 VOLUME                     ");
        break;
    }

    display.printFixed(0, 0, displayBuffer, STYLE_NORMAL);


    canvas.clear();
    canvas.setColor(0xFF);
    canvas.fillRect(0, 0, position, 5);
    display.drawCanvas(0, 20, canvas);



  } while (true);
}
