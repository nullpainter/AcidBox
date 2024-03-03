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

    // Global test
    midiHandler.sendControlChange(CC_ANY_COMPRESSOR, newPos, 1);
    midiHandler.sendControlChange(CC_ANY_REVERB_TIME, newPos, 1);
    midiHandler.sendControlChange(CC_ANY_REVERB_LVL, newPos, 1);
    midiHandler.sendControlChange(CC_ANY_DELAY_FB, newPos, 1);
    midiHandler.sendControlChange(CC_ANY_DELAY_TIME, newPos, 1);
    midiHandler.sendControlChange(CC_ANY_DELAY_LVL, newPos, 1);


    // Synth test
    midiHandler.sendControlChange(CC_303_RESO, newPos, SYNTH1_MIDI_CHAN);
    midiHandler.sendControlChange(CC_303_RESO, newPos, SYNTH2_MIDI_CHAN);
    midiHandler.sendControlChange(CC_808_DISTORTION, newPos, DRUM_MIDI_CHAN);

    pos = newPos;
  }
}