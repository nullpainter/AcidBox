#pragma once

#include <RotaryEncoder.h>
#include "Button2.h"
#include "Controller.h"
#include "MidiHandler.h"

#include "EncoderStateManager.h"

#define ROTARYSTEPS 4
#define ROTARYMIN 0

// Multiple of ROTARYSTEPS
#define ROTARYMAX 128

class EncoderHandler
{
public:
  EncoderHandler(EncoderStateManager &stateManager) : stateManager(stateManager)
  {
  }

  void setup();
  void tick();

private:
  EncoderStateManager &stateManager;

  uint8_t getValue(RotaryEncoder &encoder);
  static void handleButtonPress(Button2 &button);

  RotaryEncoder encoders[EncoderStateManager::localNumEncoders] = {
      RotaryEncoder(4, 2),
      RotaryEncoder(3, 6),
      RotaryEncoder(18, 19),
      RotaryEncoder(15, 14)};
};
