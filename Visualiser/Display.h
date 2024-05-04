#pragma once

#include <lcdgfx.h>
#include <lcdgfx_gui.h>
#include "InputBus.h"

#define DISPLAY_BUFFER_SIZE 64

static const uint8_t canvasWidth = 128;
static const uint8_t canvasHeight = 32;

class Display {
public:
  void setup();
  void update();
private:
  uint8_t canvasData[canvasWidth * (canvasHeight / 8)];

  DisplaySSD1306_128x32_I2C* display;
  NanoCanvas1* canvas;
};