#include <RotaryEncoder.h>
#include <Button2.h>
#include <midi_config.h>
#include "Visualiser.h"
#include "Encoder.h"

#define SYNTH1_MIDI_CHAN 1
#define SYNTH2_MIDI_CHAN 2

#define DRUM_MIDI_CHAN 10

void EncoderHandler::setup()
{

  // Enocder pins
  // TODO confirm if necessary
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);

  encoderState[0].button.begin(7, INPUT_PULLUP);
  encoderState[0].midiChannel = DRUM_MIDI_CHAN;
  encoderState[0].midiControlNumber = CC_808_VOLUME;

  encoderState[1].button.begin(4, INPUT_PULLUP);
  encoderState[1].midiChannel = DRUM_MIDI_CHAN;
  encoderState[1].midiControlNumber = CC_808_DISTORTION;  

  encoderState[2].button.begin(2, INPUT_PULLUP);
  encoderState[2].midiChannel = DRUM_MIDI_CHAN;
  encoderState[2].midiControlNumber = CC_808_BD_TONE;  

  encoderState[3].button.begin(0, INPUT_PULLUP);
  encoderState[3].midiChannel = SYNTH1_MIDI_CHAN;
  encoderState[3].midiControlNumber = CC_303_SATURATOR;  


  for (uint8_t i = 0; i < NUM_ENCODERS; i++)
  {
    encoderState[i].position = MIN_MIDI_VAL;
    encoderState[i].pressed = false;
  }
}

// FIXME set this to be isClamped() instead and set transmitted=false when go too high or too low
// This way the menu updates
uint8_t EncoderHandler::getValue(RotaryEncoder &encoder)
{

  long newPos = encoder.getPosition() * ROTARYSTEPS;

  if (newPos < ROTARYMIN)
  {
    encoder.setPosition(ROTARYMIN / ROTARYSTEPS);
    return ROTARYMIN;
  }

  if (newPos > ROTARYMAX)
  {
    encoder.setPosition(ROTARYMAX / ROTARYSTEPS);
    return ROTARYMAX;
  }

  // Encoder values are from 0-128 instead of 0-127 in order to result in an even multiple of
  // ROTARYSTEPS, however MIDI values are from 0-127, so we need to clamp.
  return constrain(newPos, MIN_MIDI_VAL, MAX_MIDI_VAL);
}

static uint8_t j;

void EncoderHandler::tick()
{

  for (uint8_t i = 0; i < NUM_ENCODERS; i++)
  {
    EncoderState *state = &encoderState[i];
    state->button.loop();

    encoders[i].tick();

    // Get clamped encoder value and button state
    state->position = getValue(encoders[i]);
    state->pressed = state->button.isPressed();
  }
}