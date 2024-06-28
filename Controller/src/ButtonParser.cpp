#include <usbhub.h>
#include "ButtonParser.h"
#include "MidiHandler.h"

extern MidiHandler midiHandler;

/*
 * Handles a key down event.
 */
void ButtonParser::OnKeyDown(uint8_t mod, uint8_t key)
{
    uint8_t c = OemToAscii(mod, key);

#ifdef BUTTON_DEBUG
    Serial.print("key down: ");
    Serial.print(mod);
    Serial.print(" ");
    Serial.println(key);
#endif

    SendMidiMessage(c, KEY_DOWN_CHANNEL);
}

/*
 * Handles a key up event.
 */
void ButtonParser::OnKeyUp(uint8_t mod, uint8_t key)
{
#ifdef BUTTON_DEBUG
    Serial.print("key up: ");
    Serial.print(mod);
    Serial.print(" ");
    Serial.println(key);
#endif

    uint8_t c = OemToAscii(mod, key);
    SendMidiMessage(c, KEY_UP_CHANNEL);
}

/*
 * Sends a MIDI program change message based on key up and key down events.
 */
void ButtonParser::SendMidiMessage(uint8_t key, uint8_t channel)
{

    int keyIndex = key - '1';

    if (keyIndex >= 0 && keyIndex < 9)
    {
        midiHandler.sendProgramChange(keyIndex, channel);
    }
    else
    {
        keyIndex = key - 'a';
        if (keyIndex >= 0 && keyIndex < 4)
        {
            midiHandler.sendProgramChange(keyIndex + 9, channel);
        }
    }
}

void ButtonParser::OnKeyPressed(uint8_t key) {
};