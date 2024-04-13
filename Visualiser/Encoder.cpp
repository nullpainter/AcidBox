#include <RotaryEncoder.h>
#include "Button2.h"
#include "Encoder.h"

void EncoderHandler::setup() {

  // Enocder pins
  // TODO confirm if necessary
  pinMode(2, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(20, INPUT_PULLUP);
  pinMode(21, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);

  encoderState[0].button.begin(8, INPUT_PULLUP);
  encoderState[0].midiChannel = DRUM_MIDI_CHAN;
  encoderState[0].midiControlNumber = CC_808_VOLUME;

  encoderState[1].button.begin(7, INPUT_PULLUP);
  encoderState[1].midiChannel = 1;  // Global
  encoderState[1].midiControlNumber = CC_ANY_DELAY_FB;

  encoderState[2].button.begin(9, INPUT_PULLUP);
  encoderState[2].midiChannel = 1;  // Global
  encoderState[2].midiControlNumber = CC_ANY_DELAY_TIME;

  encoderState[3].button.begin(16, INPUT_PULLUP);
  encoderState[3].midiChannel = 1;  // Global
  encoderState[3].midiControlNumber = CC_ANY_REVERB_TIME;

  for (uint8_t i = 0; i < NUM_ENCODERS; i++) {
    encoderState[i].position = MIN_MIDI_VAL;
    encoderState[i].pressed = false;
  }
}

// FIXME set this to be isClamped() instead and set transmitted=false when go too high or too low
// This way the menu updates
uint8_t EncoderHandler::getValue(RotaryEncoder& encoder) {

  long newPos = encoder.getPosition() * ROTARYSTEPS;

  if (newPos < ROTARYMIN) {
    encoder.setPosition(ROTARYMIN / ROTARYSTEPS);
    return ROTARYMIN;
  }

  if (newPos > ROTARYMAX) {
    encoder.setPosition(ROTARYMAX / ROTARYSTEPS);
    return ROTARYMAX;
  }

  // Encoder values are from 0-128 instead of 0-127 in order to result in an even multiple of 
  // ENCODERSTEPS, however MIDI values are from 0-127, so we need to clamp.
  return constrain(newPos, MIN_MIDI_VAL, MAX_MIDI_VAL);
}

static uint8_t j;

void EncoderHandler::tick(MidiHandler* midiHandler) {  // TODO pass this by reference

  for (uint8_t i = 0; i < NUM_ENCODERS; i++) {
    EncoderState* state = &encoderState[i];
    state->button.loop();

    // Get clamped encoder value
    encoders[i].tick();
    uint8_t newPos = getValue(encoders[i]);

#ifdef DEBUG
    Serial.print(newPos);
    Serial.print(" \t");
#endif

    // If the encoder has changed value, send a MIDI control change for the encoder
    if (state->position != newPos) {

      state->position = newPos;
      state->transmitted = false;

      // Send MIDI message to AcidBox
      // midiHandler->sendControlChange(state->midiControlNumber, state->position, state->midiChannel);
    }

    // Transmit encoder data if the button was pressed
    bool buttonPressed = state->button.wasPressed();
    if (state->pressed != buttonPressed) {
      state->pressed = buttonPressed;
      state->transmitted = false;
    }
  }

#ifdef DEBUG
  Serial.println();
#endif
}