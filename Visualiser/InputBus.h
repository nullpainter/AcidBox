#pragma once

#include <NoDelay.h>
#include "Encoder.h"

// Packet marker bytes
#define DATA_SEND_START 0xF1
#define DATA_SEND_END   0xE1

class InputBus {
  public:
    void update(EncoderState encoderState[]); 
    void setup();
    volatile bool packetReady = false;
  private:
    const uint8_t spiDelayMs = 50;
    noDelay spiDelay;
    uint8_t bufferPos;
};
