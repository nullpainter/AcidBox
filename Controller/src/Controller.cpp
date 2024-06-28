#include <SPI.h>
#include <hidboot.h>
#include <usbhub.h>
#include "ButtonParser.h"
#include "Controller.h"
#include "Encoder.h"
#include "EncoderStateManager.h"
#include "MidiHandler.h"
#include "InputBus.h"

EncoderStateManager encoderStateManager;
MidiHandler midiHandler(encoderStateManager);
EncoderHandler encoderHandler(encoderStateManager);
InputBus inputBus(encoderStateManager);
ButtonParser buttonParser;

USB usb;
HIDBoot<USB_HID_PROTOCOL_KEYBOARD> hidKeyboard(&usb);

void setup()
{
#ifdef DEBUG
  Serial.begin(115200);
  Serial.print("Controller initialising...");
#endif

  encoderHandler.setup();
  inputBus.setup();
  midiHandler.setup();

  // Wait for USB hub to initialise
  while (usb.Init() == -1)
  {
    delay(200);
  }

  hidKeyboard.SetReportParser(0, &buttonParser);

#ifdef DEBUG
  Serial.println("done");
#endif
}

void loop()
{
  encoderHandler.tick();
  inputBus.update();
  midiHandler.tick();
  usb.Task();
}
