#pragma once

#include <NoDelay.h>
#include "Encoder.h"

// Packet marker bytes
#define DATA_SEND_START 0xF1
#define DATA_SEND_END   0xE1

#define BUFFER_SIZE 128

class InputBus {
  public:
    void update(); 
    void setup();
    static volatile bool packetReady;
  private:
    const uint8_t spiDelayMs = 50;
    noDelay spiDelay;
};
