#include <RotaryEncoder.h>
#include "Button2.h"
#include "Encoder.h"
#include "MidiHandler.h"

void EncoderHandler::setup()
{
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

  (&stateManager.encoderState[0])->button.begin(8, INPUT_PULLUP);
  (&stateManager.encoderState[0])->midiChannel = DRUM_MIDI_CHAN;
  (&stateManager.encoderState[0])->midiControlNumber = CC_808_VOLUME;

  (&stateManager.encoderState[1])->button.begin(7, INPUT_PULLUP);
  (&stateManager.encoderState[1])->midiChannel = 1; // Global
  (&stateManager.encoderState[1])->midiControlNumber = CC_ANY_DELAY_FB;

  (&stateManager.encoderState[2])->button.begin(9, INPUT_PULLUP);
  (&stateManager.encoderState[2])->midiChannel = 1; // Global
  (&stateManager.encoderState[2])->midiControlNumber = CC_ANY_DELAY_TIME;

  (&stateManager.encoderState[3])->button.begin(16, INPUT_PULLUP);
  (&stateManager.encoderState[3])->midiChannel = 1; // Global
  (&stateManager.encoderState[3])->midiControlNumber = CC_ANY_REVERB_TIME;
}

// FIXME set this to be isClamped() instead and set transmitted=false when go too high or too low
// This way the menu updates
uint8_t EncoderHandler::getValue(RotaryEncoder &encoder)
{

  long newPos = encoder.getPosition() * ROTARYSTEPS;

  if (newPos < ROTARYMIN)
  {
    encoder.setPosition(ROTARYMIN / ROTARYSTEPS);
    newPos = ROTARYMIN;
  }

  if (newPos > ROTARYMAX)
  {
    encoder.setPosition(ROTARYMAX / ROTARYSTEPS);
    newPos = ROTARYMAX;
  }

  // Encoder values are from 0-128 instead of 0-127 in order to result in an even multiple of
  // ENCODERSTEPS, however MIDI values are from 0-127, so we need to clamp.
  return constrain(newPos, MIN_MIDI_VAL, MAX_MIDI_VAL);
}

void EncoderHandler::tick()
{
  for (uint8_t i = 0; i < EncoderStateManager::localNumEncoders; i++)
  {
    auto state = stateManager.getEncoderState(i, true);

    // Read current button state
    state->button.loop();

    // Read current rotary encoder value
    encoders[i].tick();

    // Get clamped encoder value
    auto newPos = getValue(encoders[i]);

    state->positionChanged = state->position != newPos;
    state->position = newPos;

    auto buttonPressed = state->button.wasPressed();
    state->pressed = buttonPressed;
  }
}