#pragma once

#include <NoDelay.h>
#include "Encoder.h"

// Packet marker bytes
#define DATA_SEND_START 0xF1
#define DATA_SEND_END 0xE1
#define DATA_READ 0xD1
#define DATA_READ_END 0xD2

#define INPUT_BUFFER_SIZE 128 // FIXME move to class

class InputBus
{
public:
  void update(EncoderState encoderState[]);
  void setup();

private:
  void transmit(EncoderState encoderState[]);
  void sendEncoderValues(EncoderState encoderState[]);
  void receiveEncoderValues();

  const uint8_t spiDelayMs = 100;
  noDelay spiDelay;

  uint8_t inputBuffer[INPUT_BUFFER_SIZE];
};
