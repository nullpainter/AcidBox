#include <FastLED.h>
#include <SPI.h>
#include <Ramp.h>
#include <lcdgfx.h>
#include <lcdgfx_gui.h>
#include <NoDelay.h>

#include "Silkscreen.h"
#include "libraries/midi_config.h"
#include "Animation.h"
#include "BeatDetector.h"

Display display;
ImputBus inputBus;
BeatDetector beatDefector;
ramp animationRamp;

extern CRGBPalette16 currentPalette;
extern TBlendType currentBlending;
extern CRGB leds[];

#define BUFFER_SIZE 64
uint8_t buffer[BUFFER_SIZE];


// Speed that the LEDs animate their brightness when first power on
#define ON_RAMP_SPEED 1000

DisplaySSD1306_128x32_I2C display(-1);

const uint8_t canvasWidth = 128;
const uint8_t canvasHeight = 32;

uint8_t canvasData[canvasWidth * (canvasHeight / 8)];
NanoCanvas1 canvas(canvasWidth, canvasHeight, canvasData);

void prepareSpiSlave() {

  // Turn on SPI in slave mode
  SPCR |= _BV(SPE);
  pinMode(MISO, OUTPUT);

  SPI.attachInterrupt();
}

void setup() {

  beatDetector.setup();
  beatDetector.setupADC();
  
  // TODO - this should blink, right?
  pinMode(LED_BUILTIN, OUTPUT);

  prepareSpiSlave();

  display.begin();
  display.clear();

  canvas.setFreeFont(free_slkscr10x14);
  canvas.setMode(CANVAS_MODE_TRANSPARENT);  // TODO necessary?

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
   loopBeatDetector();
   display.update(&inputBus);

  if (animationDelay.update()) {
    animate();
  }

  processInput();
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
