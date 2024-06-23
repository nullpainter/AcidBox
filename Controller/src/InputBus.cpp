#include <SPI.h>
#include <RotaryEncoder.h>
#include <NoDelay.h>

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
  // power_timer0_disable();

  spiDelay = noDelay(spiDelayMs);

  // Configure SS pins
  pinMode(INPUT_BUS_SS, OUTPUT);
  pinMode(USB_HOST_SHIELD_SS, OUTPUT);

  // Set SS high to avoid floating SS lines w/multiple slaves
  digitalWrite(INPUT_BUS_SS, HIGH);
  digitalWrite(USB_HOST_SHIELD_SS, HIGH);

  SPI.begin();
}

void InputBus::update(EncoderState encoderState[])
{

  // Delay after each packet to give the slave time to to process
  if (spiDelay.update())
  {
    transmit(encoderState);
  }
}

void InputBus::transmit(EncoderState encoderState[])
{

  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

  // Enable communication with the input bus
  digitalWrite(INPUT_BUS_SS, LOW);
  digitalWrite(USB_HOST_SHIELD_SS, HIGH);

  // Send and receive encoder values
  sendEncoderValues(encoderState);
  receiveEncoderValues();

  // Restore communication to the USB host shield
  digitalWrite(INPUT_BUS_SS, HIGH);
  digitalWrite(USB_HOST_SHIELD_SS, LOW);

  SPI.endTransaction();
}

extern MidiHandler midiHandler; // FIXME pass this in

void InputBus::sendEncoderValues(EncoderState encoderState[])
{
  // Send header
  transferAndWait(DATA_SEND_START);

  // Send data
  for (uint8_t i = 0; i < NUM_ENCODERS; i++)
  {
    transferAndWait(i);
    transferAndWait(encoderState[i].position);
    transferAndWait(encoderState[i].pressed);
    transferAndWait(encoderState[i].midiControlNumber);

    // TODO is this still required?
    // encoderState[i].transmitted = true;

    encoderState[i].button.resetPressedState();
  }

  transferAndWait(DATA_SEND_END);
}

void InputBus::receiveEncoderValues()
{
  // Response is on next transfer
  transferAndWait(DATA_READ);

  // TEMP
  uint8_t numBytes = 0;

  do
  {
    uint8_t response = transferAndWait(DATA_READ);
    if (numBytes >= INPUT_BUFFER_SIZE)
    {
      Serial.println("Buffer overrun");
      break;
    }

    inputBuffer[numBytes++] = response;

    // Ignore echoed values and end values
    if (response == DATA_READ || response == DATA_READ_END)
    {
      break;
    }

  } while (true);

  // FIXME TEMP - tidy plz
  uint8_t responseSize = 17;
  if (numBytes < responseSize)  // Partial packet due to FastLED
    return;

  for (uint8_t i = 0; i < numBytes; i++)
  {

    Serial.print(inputBuffer[i], HEX);
    Serial.print(" ");
  }

  Serial.println();
}
