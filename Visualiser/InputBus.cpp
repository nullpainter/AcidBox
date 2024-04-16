#include <SPI.h>
#include "InputBus.h"


uint8_t bufferPos;
uint8_t buffer[BUFFER_SIZE];


void InputBus::setup() {
  spiDelay = noDelay(spiDelayMs);
  InputBus::packetReady = false;
}

void InputBus::update() {
}

// SPI interrupt routine
ISR(SPI_STC_vect) {

  // Read SPI data
  byte data = SPDR;

  // Reset buffer index at the start of a packet. Subsequent packets will override the existing packet.
  if (data == DATA_SEND_START) {
    bufferPos = 0;
    return;
  }

  if (data == DATA_SEND_END) {

    // Set flag as soon as the first packet is ready. We don't need to reset this field as subsequent
    // packets override existing values in the buffer. This is just to ensure that at least one complete
    // packet is present in the buffer.

    InputBus::packetReady = true;
  }

  // add payload to buffer if room
  if (bufferPos < sizeof buffer) {
    buffer[bufferPos++] = data;
  }
}
