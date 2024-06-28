#include <midi_config.h>  // TODO move this centrally between visualiser and display
#include "InputBus.h"
#include "Display.h"
#include "Silkscreen.h"

void Display::setup() {
  display = new DisplaySSD1306_128x32_I2C(-1);
  canvas = new NanoCanvas1(canvasWidth, canvasHeight, canvasData);

  display->begin();
  display->clear();

  canvas->setFreeFont(free_slkscr10x14);
  canvas->setMode(CANVAS_MODE_TRANSPARENT);  // TODO necessary?
}

void Display::update() {

  // Nothing to do if we don't have a full packet
  if (!InputBus::packetReady) return;

  uint8_t i = 0;
  char displayBuffer[DISPLAY_BUFFER_SIZE];
  displayBuffer[0] = 0x0;

  do {

    // Break if we have read all encoders
    if (InputBus::buffer[i] == DATA_SEND_END)
      break;

    if (i >= BUFFER_SIZE) {
      Serial.println("Error - input buffer overrun");
      return;
    }

    uint8_t position = InputBus::buffer[i++];

    uint8_t pressed = InputBus::buffer[i++];
    uint8_t midiControlNumber = InputBus::buffer[i++];

/// FIXME all of this needs an overhaul. Need to look at both slave and master data. Suspect this is causign the whole thing to just bork.

    switch (midiControlNumber) {
      case CC_ANY_DELAY_FB:
        sprintf(displayBuffer, "DELAY FEEDBACK");
        break;
      case CC_ANY_DELAY_TIME:
        sprintf(displayBuffer, "DELAY TIME");
        break;
      case CC_808_VOLUME:
        sprintf(displayBuffer, "808 VOLUME");
        Serial.print("VOL ");
        Serial.println(position);
        break;
      case CC_ANY_REVERB_LVL:
        sprintf(displayBuffer, "REVERB LEVEL");
        break;
      case CC_ANY_REVERB_TIME:
        sprintf(displayBuffer, "REVERB TIME");
        break;
      default:
        sprintf(displayBuffer, "MYSTERY %d", midiControlNumber);
        break;
    }

    canvas->clear();
    canvas->printFixed(0, 0, displayBuffer, STYLE_NORMAL);
    canvas->fillRect(0, 20, position, 25);

    display->drawCanvas(0, 0, *canvas);

  } while (true);

  InputBus::packetReady = false;
}