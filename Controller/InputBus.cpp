#include <SPI.h>
#include <RotaryEncoder.h>
#include <NoDelay.h>

#include "InputBus.h"
#include "Encoder.h"
#include "Devices.h"

void InputBus::setup() {

  spiDelay = noDelay(spiDelayMs);

  // Configure SS pins
  pinMode(INPUT_BUS_SS, OUTPUT);
  pinMode(USB_HOST_SHIELD_SS, OUTPUT);

  // Set SS high to avoid floating SS lines w/multiple slaves
  digitalWrite(INPUT_BUS_SS, HIGH);
  digitalWrite(USB_HOST_SHIELD_SS, HIGH);

  SPI.begin();
}

// TODO rename this to update - tick is dumb
void InputBus::tick(EncoderState encoderState[]) {

  // Delay after each packet to give the slave time to to process
  if (spiDelay.update()) {
    transmit(encoderState);
  }
}

void InputBus::transmit(EncoderState encoderState[]) {

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

void InputBus::receiveEncoderValues() {
  /*
  uint8_t remoteKnobCount = SPI.transfer(DATA_READ_START);

  printf("Read %d remote knobs\n", remoteKnobCount);

  if (remoteKnobCount >= NUM_REMOTE_KNOBS) {
    printf("Unexpected remote knob count: %d\n");
    return;
  }*/

  // FIXME reinstate w/struct. Also use in Encoder class

  /*
  for (uint8_t i = 0; i < remoteKnobCount; i++) {
    remoteKnobValues[i] = SPI.transfer(0);
    remoteButtonValues[i] = SPI.transfer(0);

    printf("Remote knob %d value: %d, state: %d", i, remoteKnobValues[i], remoteButtonValues[i]);
  }
  */
}

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
  }

  SPI.transfer(DATA_SEND_END);
}
