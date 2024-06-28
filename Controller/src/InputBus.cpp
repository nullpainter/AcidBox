#include <SPI.h>
#include <RotaryEncoder.h>
#include <NoDelay.h>

#include "EncoderStateManager.h"
#include "InputBus.h"
#include "Encoder.h"
#include "Devices.h"

// TODO move into class
byte transferAndWait(const uint8_t what)
{
  byte a = SPI.transfer(what);
  delayMicroseconds(20);
  return a;
}

void InputBus::setup()
{
  spiDelay = noDelay(spiDelayMs);

  // Configure SS pins
  pinMode(INPUT_BUS_SS, OUTPUT);
  pinMode(USB_HOST_SHIELD_SS, OUTPUT);

  // Set SS high to avoid floating SS lines w/multiple slaves
  digitalWrite(INPUT_BUS_SS, HIGH);
  digitalWrite(USB_HOST_SHIELD_SS, HIGH);

  SPI.begin();
}

void InputBus::update()
{
  // Delay after each packet to give the slave time to to process
  if (spiDelay.update())
  {
    transmit();
  }
}

void InputBus::transmit()
{
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

  // Enable communication with the input bus
  digitalWrite(INPUT_BUS_SS, LOW);
  digitalWrite(USB_HOST_SHIELD_SS, HIGH);

  // Send and receive encoder values
  sendEncoderValues();
  bool packetReceived = receiveEncoderValues();

  // Restore communication to the USB host shield
  digitalWrite(INPUT_BUS_SS, HIGH);
  digitalWrite(USB_HOST_SHIELD_SS, LOW);

  SPI.endTransaction();

  // Set remote encoder state
  if (packetReceived)
  {
    SetRemoteEncoderState();
  }
}

void InputBus::sendEncoderValues()
{
  // Send header
  transferAndWait(DATA_SEND_START);
  auto encoderState = stateManager.encoderState;

  // Send data
  for (uint8_t i = 0; i < EncoderStateManager::localNumEncoders; i++)
  {
    transferAndWait((&encoderState[i])->position);
    transferAndWait((&encoderState[i])->pressed);
    transferAndWait((&encoderState[i])->midiControlNumber);

    // TEMP
    // (&encoderState[i])->button.resetPressedState();
  }

  transferAndWait(DATA_SEND_END);
}

/**
 * Receives the values of the encoders from the visualiser module.
 *
 * @return true if a packet has been received.
 */
bool InputBus::receiveEncoderValues()
{
  // Response is on next transfer
  transferAndWait(DATA_READ);

  uint8_t bytesReceived = 0;

  do
  {
    auto response = transferAndWait(DATA_READ);

    // Ignore echoed data from the controller
    if (response == DATA_READ)
    {
      continue;
    }

    // Break when we have all bytes
    if (response == DATA_READ_END)
    {
      break;
    }

    // Verify that we haven't received more bytes than the buffer can hold
    if (bytesReceived >= inputBufferSize)
    {
      Serial.println(F("Buffer overrun"));
      break;
    }

    inputBuffer[bytesReceived++] = response;
  } while (true);

  // FastlED disables interrupts when updating animations. This manifests itself in partial
  // packets, so we can just disregard them.
  return bytesReceived == responseByteCount;
}

void InputBus::SetRemoteEncoderState()
{
  // Populate remote encoder state
  uint8_t i = 0;
  for (uint8_t encoderIndex = 0; encoderIndex < EncoderStateManager::remoteNumEncoders; encoderIndex++)
  {
    auto state = stateManager.getEncoderState(encoderIndex, false);
    auto position = inputBuffer[i++];

    state->positionChanged = state->position != position;
    state->position = position;

    state->pressed = inputBuffer[i++];
    state->midiControlNumber = inputBuffer[i++];
    state->midiChannel = inputBuffer[i++];
  }
}
