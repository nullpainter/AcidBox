#pragma once

#include "Button2.h"

struct EncoderState
{
  uint8_t position;
  bool pressed;
  uint8_t midiControlNumber;
  uint8_t midiChannel;
  // Button2 button; // TEMP

  // If the encoder's position has changed from its previous state
  bool positionChanged;
};