#pragma once

#include <NoDelay.h>
#include "Encoder.h"

// TODO make these guys common - currently copied and pasted

// Packet marker bytes
#define DATA_SEND_START 0xF1
#define DATA_SEND_END   0xE1
#define DATA_READ       0xD1
#define DATA_READ_END   0xD2

#define BUFFER_SIZE 512 

// Number of bytes sent/received per encoder
#define BYTES_PER_ENCODER 4


class InputBus {
public:
  void update();
  void setup();
  inline static volatile bool packetReady;
  inline static uint8_t bufferPos;
  inline static uint8_t buffer[BUFFER_SIZE];
  
  inline static volatile uint8_t outputBufferPos;

private:
  const uint8_t spiDelayMs = 5;
  noDelay spiDelay;
};