#pragma once

#include <RotaryEncoder.h>
#include "Button2.h"

#define MIN_MIDI_VAL 0
#define MAX_MIDI_VAL 127

#define NUM_ENCODERS 1

#define ROTARYSTEPS 4
#define ROTARYMIN 0

// Multiple of ROTARYSTEPS
#define ROTARYMAX 128 

struct EncoderState {
  uint8_t position;
  bool pressed;
  uint8_t midiControlNumber;
  uint8_t midiChannel;
  Button2 button;

  bool transmitted;
};

class EncoderHandler {
public:
  void setup();
  void tick();
  inline static EncoderState encoderState[NUM_ENCODERS];

private:
  uint8_t getValue(RotaryEncoder& encoder);
  static void handleButtonPress(Button2& button);
  RotaryEncoder encoders[NUM_ENCODERS] = {
    RotaryEncoder(8, 9)
  };
};
