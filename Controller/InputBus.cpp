#include <SPI.h>
#include <RotaryEncoder.h>
#include <NoDelay.h>

#include "InputBus.h"
#include "Encoder.h"
#include "Devices.h"

void InputBus::setup() {

  //power_timer0_disable();

  spiDelay = noDelay(spiDelayMs);

  // Configure SS pins
  pinMode(INPUT_BUS_SS, OUTPUT);
  pinMode(USB_HOST_SHIELD_SS, OUTPUT);

  // Set SS high to avoid floating SS lines w/multiple slaves
  digitalWrite(INPUT_BUS_SS, HIGH);
  digitalWrite(USB_HOST_SHIELD_SS, HIGH);

  SPI.begin();
}

void InputBus::update(EncoderState encoderState[]) {

  // Delay after each packet to give the slave time to to process
  if (spiDelay.update()) {
    transmit(encoderState);
  }
}

void InputBus::transmit(EncoderState encoderState[]) {

  bool transmitRequired = false;

  // TODO we need to fix / be aware of this when reading from the slave, as we always
  // want to do that. 
  for (uint8_t i = 0; i < NUM_ENCODERS; i++) {
    if (!encoderState[i].transmitted) {
      transmitRequired = true;
      break;
    }
  }

  if (!transmitRequired) return;

  SPI.beginTransaction(SPISettings(1000, MSBFIRST, SPI_MODE0));

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

extern MidiHandler midiHandler;  // FIXME pass this in

void InputBus::sendEncoderValues(EncoderState encoderState[]) {

  // Send header
  SPI.transfer(DATA_SEND_START);

  // Send data
  for (uint8_t i = 0; i < NUM_ENCODERS; i++) {

    // Only send encoder state if it has changed
    if (encoderState[i].transmitted) continue;

    SPI.transfer(i);
    SPI.transfer(encoderState[i].position);
    SPI.transfer(encoderState[i].pressed);
    SPI.transfer(encoderState[i].midiControlNumber);

    encoderState[i].transmitted = true;

    encoderState[i].button.resetPressedState();
  }

  SPI.transfer(DATA_SEND_END);
}
