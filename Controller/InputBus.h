#pragma once

#include <NoDelay.h>
#include "Encoder.h"

// Marker bytes
#define DATA_SEND_START 0xF1  // 241
#define DATA_SEND_END   0xE1  // 225

class InputBus {
  public:
    void tick(EncoderState encoderState[]); 
    void setup();
  private:
    void transmit(EncoderState encoderState[]);
    void sendEncoderValues(EncoderState encoderState[]);
    void receiveEncoderValues();

    const uint8_t spiDelayMs = 50;
    noDelay spiDelay;
};
