#include <FastLED.h>
#include <SPI.h>
#include <Ramp.h>
#include <lcdgfx.h>
#include <lcdgfx_gui.h>
#include <NoDelay.h>

#include "libraries/midi_config.h"
#include "Animation.h"
#include "Display.h"
#include "Encoder.h"

//#include "BeatDetector.h"

Display display;
InputBus inputBus;
EncoderHandler encoderHandler;

//BeatDetector beatDefector;
ramp animationRamp;

extern CRGBPalette16 currentPalette;
extern TBlendType currentBlending;
extern CRGB leds[];

#define BUFFER_SIZE 64
uint8_t buffer[BUFFER_SIZE];


// Speed that the LEDs animate their brightness when first power on
#define ON_RAMP_SPEED 1000

void prepareSpiSlave() {

  // Turn on SPI in slave mode
  SPCR |= _BV(SPE);
  pinMode(MISO, OUTPUT);

  SPI.attachInterrupt();
}

void setup() {
/*
  beatDetector.setup();
  beatDetector.setupADC();
  
  // TODO - this should blink, right?
  pinMode(LED_BUILTIN, OUTPUT);
*/

  prepareSpiSlave();
  display.setup();

  animationRamp.go(BRIGHTNESS, ON_RAMP_SPEED, CUBIC_IN, ONCEFORWARD);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(0);

  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;


  Serial.begin(115200);
  while (!Serial) {}

  Serial.println("Starting visualiser");
}
noDelay animationDelay = noDelay(1000 / UPDATES_PER_SECOND);

void loop() {
  encoderHandler.tick();

   //loopBeatDetector();
   display.update(inputBus);

  if (animationDelay.update()) {
    animate();
  }

  inputBus.update();//encoderHandler.encoderState);
}

uint8_t encoderPos;

void animate() {

  if (!animationRamp.isFinished()) {
    int brightness = animationRamp.update();
    FastLED.setBrightness(brightness);
  }

  FillLEDsFromPaletteColors(encoderPos * 2);
  FastLED.show();
}
