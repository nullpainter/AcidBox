#include <SPI.h>
// #include <hidboot.h>
// #include <usbhub.h>
// #include "ButtonParser.h"
#include "Controller.h"
#include "Encoder.h"
#include "EncoderStateManager.h"
#include "MidiHandler.h"
#include "InputBus.h"

EncoderStateManager encoderStateManager;
MidiHandler midiHandler(encoderStateManager);
EncoderHandler encoderHandler(encoderStateManager);
InputBus inputBus(encoderStateManager);
// ButtonParser buttonParser;

// TODO move these to the ESP32
// USB usb;
// HIDBoot<USB_HID_PROTOCOL_KEYBOARD> hidKeyboard(&usb);

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

  encoderHandler.tick();
  inputBus.update();
  midiHandler.tick();

  // // usb.Task(); TEMP
}
