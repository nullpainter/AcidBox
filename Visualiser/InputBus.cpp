#include <SPI.h>
#include "InputBus.h"


void InputBus::setup() {
  spiDelay = noDelay(spiDelayMs);
  InputBus::packetReady = false;
}

void InputBus::update() {
}

// SPI interrupt routine
ISR(SPI_STC_vect) {

  InputBus::packetReady = false;

  // Read SPI data
  byte data = SPDR;

  // Reset buffer index at the start of a packet. Subsequent packets will override the existing packet.
  if (data == DATA_SEND_START) {
    InputBus::bufferPos = 0;
    return;
  }

  if (data == DATA_SEND_END) {

    // Set flag as soon as the first packet is ready. We don't need to reset this field as subsequent
    // packets override existing values in the buffer. This is just to ensure that at least one complete
    // packet is present in the buffer.
    InputBus::packetReady = true;
  }

  if (data == DATA_READ) {
    /*
    TODO: Send encoder data back, ONE byte each time this is called. So we need a write pointer
    Then send back DATA_READ_END, I guess

    And the master keeps sending DATA_READ until it gets DATA_READ_END back
    */


  } else {

    // add payload to buffer if room
    if (InputBus::bufferPos < BUFFER_SIZE) {
      InputBus::buffer[InputBus::bufferPos++] = data;
    }
  }
}
