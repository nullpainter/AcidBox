#pragma once

// Copied from AcidBox - TODO move to lib
#include <midi_config.h>

// #define DEBUG
#define ENABLE_MIDI

// TODO move this stuff to the midi class

// MIDI channels for keyup and keydown events
#define KEY_DOWN_CHANNEL 1
#define KEY_UP_CHANNEL 2

// MIDI value ranges
#define MIN_MIDI_VAL 0
#define MAX_MIDI_VAL 127

#define SYNTH1_MIDI_CHAN 1
#define SYNTH2_MIDI_CHAN 2

#define DRUM_MIDI_CHAN 10