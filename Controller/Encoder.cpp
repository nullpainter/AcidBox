#include <RotaryEncoder.h>
#include "Button2.h"
#include "Encoder.h"
#include "MidiHandler.h"


void EncoderHandler::setup() {

  pinMode(2, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);


  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);


  encoderState[0].button.begin(7);
  encoderState[0].midiChannel = 1;  // Global
  encoderState[0].midiControlNumber = CC_ANY_DELAY_FB;

  encoderState[1].button.begin(8);
  encoderState[1].midiControlNumber = CC_808_VOLUME;
  encoderState[1].midiChannel = DRUM_MIDI_CHAN;


  for (uint8_t i = 0; i < NUM_ENCODERS; i++) {
    encoderState[i].position = MIN_MIDI_VAL;
    encoderState[i].pressed = false;
  }
}

uint8_t EncoderHandler::getValue(RotaryEncoder& encoder) {

  long newPos = encoder.getPosition();

  if (newPos < MIN_MIDI_VAL) {
    encoder.setPosition(MIN_MIDI_VAL);
    newPos = MIN_MIDI_VAL;
  }

  if (newPos > MAX_MIDI_VAL) {
    encoder.setPosition(MAX_MIDI_VAL);
    newPos = MAX_MIDI_VAL;
  }

  newPos = constrain(newPos * 4, MIN_MIDI_VAL, MAX_MIDI_VAL);

  return newPos;
}

static uint8_t j;

void EncoderHandler::tick(MidiHandler* midiHandler) {  // TODO pass this by reference

  for (uint8_t i = 0; i < NUM_ENCODERS; i++) {
    EncoderState* state = &encoderState[i];
    state->button.loop();

    // Get clamped encoder value
    encoders[i].tick();
    uint8_t newPos = getValue(encoders[i]);

    // If the encoder has changed value, send a MIDI control change for the encoder
    if (state->position != newPos) {

      state->position = newPos;
      state->transmitted = false;

      // Send MIDI message to AcidBox
      midiHandler->sendControlChange(state->midiControlNumber, state->position, state->midiChannel);
    }

    state->pressed = state->button.read(true) != empty;
Serial.print("Pressed ");
Serial.println(state->pressed);

    // Transmit encoder data if the button was pressed
    if (state->pressed) {
      state->transmitted = false;
    }
  }
}