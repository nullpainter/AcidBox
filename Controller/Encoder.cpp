#include "Encoder.h"
#include "MidiHandler.h"

extern MidiHandler midiHandler;

// FIXME TEMP - this only supports a single encoder
static int pos = MIN_MIDI_VAL;

// Test encoder
RotaryEncoder encoder1(4, 2, RotaryEncoder::LatchMode::TWO03);

void EncoderHandler::setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
}

void EncoderHandler::tick() {

  encoder1.tick();

  // FIXME this is a proof of concept. Need to move pos etc. into new class with an
  // instance per encoder.
  int newPos = encoder1.getPosition();
  if (newPos < MIN_MIDI_VAL) {
    encoder1.setPosition(MIN_MIDI_VAL);
    newPos = MIN_MIDI_VAL;
  }

  if (newPos > MAX_MIDI_VAL) {
    encoder1.setPosition(MAX_MIDI_VAL);
    newPos = MAX_MIDI_VAL;
  }

  if (pos != newPos) {
    midiHandler.sendControlChange(CC_ANY_REVERB_LVL, newPos, 1);
    pos = newPos;
  }
}