#include <RotaryEncoder.h>
#include "Encoder.h"
#include "MidiHandler.h"

#define LATCH_MODE RotaryEncoder::LatchMode::TWO03

// FIXME move into class
RotaryEncoder* encoders[NUM_ENCODERS] = {
  &RotaryEncoder(4, 2, LATCH_MODE)
};


void EncoderHandler::setup() {

  // TODO consolidate this and the pin values above
  pinMode(2, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);

  for (uint8_t i = 0; i < NUM_ENCODERS; i++) {
    encoderState[i].position = MIN_MIDI_VAL;
    encoderState[i].pressed = false;
  }

  encoderState[0].midiControlNumber = CC_ANY_DELAY_FB;
}

uint8_t EncoderHandler::getValue(RotaryEncoder* encoder) {

  long newPos = encoder->getPosition();

  if (newPos < MIN_MIDI_VAL) {
    encoder->setPosition(MIN_MIDI_VAL);
    newPos = MIN_MIDI_VAL;
  }

  if (newPos > MAX_MIDI_VAL) {
    encoder->setPosition(MAX_MIDI_VAL);
    newPos = MAX_MIDI_VAL;
  }

  return newPos;
}

static uint8_t j;

void EncoderHandler::tick(MidiHandler *midiHandler) {

  for (uint8_t i = 0; i < NUM_ENCODERS; i++) {
    RotaryEncoder* encoder = encoders[i];
    EncoderState* state = &encoderState[i];

    encoder->tick();

    // Get clamped encoder value
    uint8_t newPos = getValue(encoder);

    // If the encoder has changed value, send a MIDI control change for the encoder
    if (state->position != newPos) {

      state->position = newPos;
      state->transmitted = false;

//Serial.println("DONKEYS");
      midiHandler->sendControlChange(93, (j++ % 127), 1);

      // Send MIDI message to AcidBox
    //  midiHandler->sendControlChange(state->midiControlNumber, i++, 1);
    }
  }
}