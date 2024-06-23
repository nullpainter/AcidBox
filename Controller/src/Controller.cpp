#include <SPI.h>
#include <hidboot.h>
#include <usbhub.h>
#include "ButtonParser.h"
#include "Controller.h"
#include "Encoder.h"
#include "MidiHandler.h"
#include "InputBus.h"

MidiHandler midiHandler;
EncoderHandler encoderHandler;
ButtonParser buttonParser;
USB usb;
InputBus inputBus;

HIDBoot<USB_HID_PROTOCOL_KEYBOARD> hidKeyboard(&usb);

void setup()
{
#ifdef DEBUG
  Serial.begin(115200);
#endif

  encoderHandler.setup();
  inputBus.setup();
  midiHandler.setup();
  /*
    if (usb.Init() == -1) {
      Serial.begin(115200);
      Serial.println("OSC did not start.");
    }
  */
  // delay(200);
  // TEMP hidKeyboard.SetReportParser(0, &buttonParser);
}

void loop()
{

  encoderHandler.tick(&midiHandler);
  midiHandler.tick();
  // usb.Task(); TEMP
  inputBus.update(encoderHandler.encoderState);
}
