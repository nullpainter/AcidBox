#pragma once

#include <MIDI.h>
#include <hidboot.h>
#include "Controller.h"

// #define BUTTON_DEBUG

class ButtonParser : public KeyboardReportParser
{
protected:
  void OnKeyDown(uint8_t mod, uint8_t key);
  void OnKeyUp(uint8_t mod, uint8_t key);
  void OnKeyPressed(uint8_t key);

private:
  void SendMidiMessage(uint8_t key, uint8_t channel);
};