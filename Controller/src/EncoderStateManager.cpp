#include "EncoderStateManager.h"

/**
 * Retrieves the encoder state based on the given index and whether the encoder is local or remote.
 *
 * @param index The index of the encoder.
 * @param local True if the encoder is local, false otherwise.
 *
 * @return A pointer to the encoder state.
 */
EncoderState *EncoderStateManager::getEncoderState(uint8_t index, bool local)
{

  uint8_t globalIndex = local ? index : index + EncoderStateManager::remoteNumEncoders;
  return &EncoderStateManager::encoderState[globalIndex];
}

/**
 * Prints the state of all encoders for debugging.
 */
void EncoderStateManager::printEncoderState()
{
  for (uint8_t i = 0; i < EncoderStateManager::totalEncoders; i++)
  {
    auto state = &encoderState[i];

    Serial.print(state->position);
    Serial.print(" ");
    Serial.print(state->pressed);
    Serial.print(" ");
  }

  Serial.println();
}