#pragma once

#include <MIDI.h>
#include "EncoderStateManager.h"

class MidiHandler
{
public:
  MidiHandler(EncoderStateManager &stateManager) : stateManager(stateManager)
  {
  }

  void setup();
  void tick();

  /**
   * Sends MIDI control change messages for all encoders whose position has changed.
   */
  void sendEncoderControlChange();
  void sendProgramChange(uint8_t program, uint8_t channel);
  void sendControlChange(uint8_t number, uint8_t value, uint8_t channel);

  EncoderStateManager &stateManager;
};