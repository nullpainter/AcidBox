#include <LibPrintf.h>
#include "MidiHandler.h"
#include "Controller.h"

// Create and bind the MIDI interface to the default hardware Serial port
MIDI_CREATE_INSTANCE(HardwareSerial, Serial, MIDI);

void MidiHandler::setup() {
 // MIDI.begin(MIDI_CHANNEL_OMNI);
}

void MidiHandler::tick() {
 /// MIDI.read();
}

void MidiHandler::sendProgramChange(uint8_t program, uint8_t channel) {
  
#ifdef DEBUG
  printf("Program change: %d %d\n", program, channel);
#endif

 // MIDI.sendProgramChange(program, channel);
}

void MidiHandler::sendControlChange(uint8_t number, uint8_t value, uint8_t channel) {

#ifdef DEBUG
  printf("Control change: %d %d\n", number, value, channel);
#endif

  //MIDI.sendControlChange(number, value, channel);
}