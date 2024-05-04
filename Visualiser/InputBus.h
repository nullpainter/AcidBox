#pragma once

#include <NoDelay.h>
#include "Encoder.h"

// Packet marker bytes
#define DATA_SEND_START 0xF1
#define DATA_SEND_END 0xE1
#define DATA_READ 0xD1
#define DATA_READ_END 0xD1

#define BUFFER_SIZE 128

class InputBus {
public:
  void update();
  void setup();
  inline static volatile bool packetReady;
  inline static uint8_t bufferPos;
  inline static uint8_t buffer[BUFFER_SIZE];

private:
  const uint8_t spiDelayMs = 50;
  noDelay spiDelay;
};