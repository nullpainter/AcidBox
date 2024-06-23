#include <SPI.h>
#include <Ramp.h>
#include <lcdgfx.h>
#include <lcdgfx_gui.h>
#include <NoDelay.h>
#include <midi_config.h>

#include "Animation.h"
#include "Display.h"
#include "Encoder.h"

Display display;
InputBus inputBus;
EncoderHandler encoderHandler;

ramp animationRamp;

extern CRGBPalette16 currentPalette;
extern TBlendType currentBlending;
extern CRGB leds[];

// Speed that the LEDs animate their brightness when first power on
#define ON_RAMP_SPEED 1000

void prepareSpiSlave()
{

  pinMode(MISO, OUTPUT);

  // Turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // turn on interrupts
  SPCR |= _BV(SPIE);
}

void setup()
{
  /*
    // TODO - this should blink, right?
    pinMode(LED_BUILTIN, OUTPUT);
  */

  prepareSpiSlave();

  encoderHandler.setup();
  inputBus.setup();
  display.setup();

  animationRamp.go(BRIGHTNESS, ON_RAMP_SPEED, CUBIC_IN, ONCEFORWARD);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(0);

  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;

  Serial.begin(115200);
  while (!Serial)
  {
  }

  Serial.println("Starting visualiser");
}
noDelay animationDelay = noDelay(1000 / UPDATES_PER_SECOND);

uint8_t encoderPos;

void animate()
{

  if (!animationRamp.isFinished())
  {
    int brightness = animationRamp.update();
    FastLED.setBrightness(brightness);
  }

  ChangePalettePeriodically();

    static uint8_t startIndex = 0;
    startIndex++;
    
    FillLEDsFromPaletteColors( startIndex);
    FastLED.show();
}

void loop()
{
  encoderHandler.tick();
  // FIXME borked
  //  display.update(); // TODO need to fix so it only updates if the state has actually changed

  if (animationDelay.update())
  {
    animate();
  }

  inputBus.update();
}