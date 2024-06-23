#pragma once

#include <MIDI.h>

class MidiHandler {
  public:
    void setup();
    void tick();
    void sendProgramChange(uint8_t program, uint8_t channel);
    void sendControlChange(uint8_t number, uint8_t value, uint8_t channel);
};