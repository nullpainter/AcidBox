#include "MidiHandler.h"
#include "Controller.h"

// Create and bind the MIDI interface to the default hardware Serial port
MIDI_CREATE_INSTANCE(HardwareSerial, Serial, MIDI);

void MidiHandler::setup()
{
#ifdef ENABLE_MIDI
  MIDI.begin(MIDI_CHANNEL_OMNI);
#endif
}

void MidiHandler::tick()
{
#ifdef ENABLE_MIDI
  MIDI.read();
#endif

  sendEncoderControlChange();
}

/**
 * Sends MIDI control change messages for all encoders whose position has changed.
 */
void MidiHandler::sendEncoderControlChange()
{
  for (uint8_t i; i < EncoderStateManager::totalEncoders; i++)
  {
    auto state = &stateManager.encoderState[i];

    if (!state->positionChanged)
    {
      continue;
    }

    // If the encoder has changed value, send a MIDI control change for the encoder to Acidbox
    sendControlChange(state->midiControlNumber, state->position, state->midiChannel);

    state->positionChanged = false;
  }
}

void MidiHandler::sendProgramChange(uint8_t program, uint8_t channel)
{
#ifdef DEBUG
  Serial.print("Program change: ");
  Serial.print(program);
  Serial.print(" ");
  Serial.println(channel);
#endif

#ifdef ENABLE_MIDI
  MIDI.sendProgramChange(program, channel);
#endif
}

void MidiHandler::sendControlChange(uint8_t number, uint8_t value, uint8_t channel)
{

#ifdef DEBUG
  Serial.print("Control change:");
  Serial.print(number);
  Serial.print(" ");
  Serial.print(value);
  Serial.print(" ");
  Serial.print(channel);
  Serial.println();

  // printf("Control change: %d %d %d\n", number, value, channel);
#endif

#ifdef ENABLE_MIDI
  MIDI.sendControlChange(number, value, channel);
#endif
}