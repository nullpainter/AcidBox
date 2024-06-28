#include <SPI.h>
#include "InputBus.h"
#include "Encoder.h"

void InputBus::setup()
{
  spiDelay = noDelay(spiDelayMs);
  InputBus::packetReady = false;

  InputBus::bufferPos = 0;
  InputBus::outputBufferPos = 0;
}

void InputBus::update()
{
  if (digitalRead(SS) == HIGH)
  {
    InputBus::outputBufferPos = 0;
  }
}
volatile uint8_t j = 0;
volatile uint8_t i = 0;

// SPI interrupt routine
ISR(SPI_STC_vect)
{
  InputBus::packetReady = false;

  // Read SPI data
  byte data = SPDR;

  switch (data)
  {
  case DATA_SEND_START:

    // Reset buffer index at the start of a packet. Subsequent packets will override the existing packet.
    InputBus::bufferPos = 0;
    break;

  case DATA_SEND_END:
    // Set flag as soon as the first packet is ready. We don't need to reset this field as subsequent
    // packets override existing values in the buffer. This is just to ensure that at least one complete
    // packet is present in the buffer.
    InputBus::packetReady = true;
    break;
  case DATA_READ:

    uint8_t encoderDataIndex = InputBus::outputBufferPos % BYTES_PER_ENCODER;
    uint8_t encoderIndex = floor(InputBus::outputBufferPos / BYTES_PER_ENCODER);

    // Send end read marker once we've sent all encoder values
    if (encoderIndex >= NUM_ENCODERS)
    {
      SPDR = DATA_READ_END;
      return;
    }

    EncoderState *state = &EncoderHandler::encoderState[encoderIndex];
    switch (encoderDataIndex)
    {
    case 0:
      SPDR = state->position;
      break;
    case 1:
      SPDR = state->pressed;
      break;
    case 2:
      SPDR = state->midiControlNumber;
      break;
    case 3:
      SPDR = state->midiChannel;
      break;
    }

    InputBus::outputBufferPos++;
    break;
  default:

    // // Populate read buffer - add payload to buffer if room
    if (InputBus::bufferPos < BUFFER_SIZE)
    {
      InputBus::buffer[InputBus::bufferPos++] = data;
    }
    break;
  }
}