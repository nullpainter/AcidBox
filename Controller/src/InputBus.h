#pragma once

#include <NoDelay.h>
#include "Encoder.h"

// Packet marker bytes
#define DATA_SEND_START 0xF1
#define DATA_SEND_END 0xE1
#define DATA_READ 0xD1
#define DATA_READ_END 0xD2

class InputBus
{
public:
  InputBus(EncoderStateManager &stateManager) : stateManager(stateManager)
  {
  }

  void update();
  void setup();

  const static uint8_t inputBufferSize = 16;

  // Buffer to store the response from the visualiser
  uint8_t inputBuffer[inputBufferSize];

private:
  void transmit();
  void sendEncoderValues();

  /**
   * Receives the values of the encoders from the visualiser module.
   */
  void receiveEncoderValues();

  const uint8_t bytesPerEncoder = 3;

  // Number of bytes sent by the visualiser in a single packet
  const uint8_t responseByteCount = bytesPerEncoder * EncoderStateManager::remoteNumEncoders;

  // Delay in ms between packets sent to the visualiser in order to give it time to respond
  const uint8_t spiDelayMs = 100;
  noDelay spiDelay;

  EncoderStateManager &stateManager;
};