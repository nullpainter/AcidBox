#pragma once

#include <RotaryEncoder.h>
#include "Controller.h"
#include "MidiHandler.h"

#define NUM_ENCODERS 1

struct EncoderState {
  uint8_t position;
  bool pressed;
  uint8_t midiControlNumber;
  bool transmitted;
};

class EncoderHandler {
public:
  void setup();
  void tick(MidiHandler *midiHandler);
  EncoderState encoderState[NUM_ENCODERS];

private:
  uint8_t getValue(RotaryEncoder *encoder);
};
