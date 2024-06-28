#pragma once

#include "Arduino.h"
#include "EncoderState.h"

class EncoderStateManager
{
public:
    static const uint8_t localNumEncoders = 4;
    static const uint8_t remoteNumEncoders = 4;

    static const uint8_t totalEncoders = localNumEncoders + remoteNumEncoders;

    EncoderState encoderState[totalEncoders];

    /**
     * Retrieves the encoder state based on the given index and whether the encoder is local or remote.
     *
     * @param index The index of the encoder.
     * @param local True if the encoder is local, false otherwise.
     *
     * @return A pointer to the encoder state.
     */
    EncoderState *getEncoderState(uint8_t index, bool local);

    /**
     * Prints the state of all encoders for debugging.
     */
    void printEncoderState();
};