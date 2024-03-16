#pragma once

#include <RotaryEncoder.h>
#include "Button2.h"
#include "Controller.h"
#include "MidiHandler.h"

#define NUM_ENCODERS 2

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
  void tick(MidiHandler* midiHandler);
  EncoderState encoderState[NUM_ENCODERS];

private:
  uint8_t getValue(RotaryEncoder& encoder);
  static void handleButtonPress(Button2& button);
  RotaryEncoder encoders[NUM_ENCODERS] = {
    RotaryEncoder(4, 2),  
    RotaryEncoder(3, 6) 
  };
};
