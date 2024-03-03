void setButtonState(uint8_t number, bool pressed) {
  uint8_t i;

  buttonEvent* previous;
  buttonEvent* current;

  // Rotate historical button presses
  for (i = 1; i < BUTTON_HISTORY_SIZE; i++) {
    previous = &button_history[i - 1];
    current = &button_history[i];

    previous->number = current->number;
    previous->pressed = current->pressed;
    previous->processed = current->processed;
  }

  // Register current button press
  current->number = number;
  current->pressed = pressed;
  current->processed = false;

  //printButtonHistory();
}

int8_t getButtonPressed() {

  // Identify button up, button down
  // prvent premature firing on three button presses due to holding
  if (button_history[2].pressed && !button_history[2].processed && !button_history[3].pressed && !button_history[3].processed && button_history[2].number == button_history[3].number && !button_history[1].pressed) {

    // Mark both as processed so we only process the button press once
    button_history[2].processed = true;
    button_history[3].processed = true;

//    Serial.println("PRESSED");
 //   printButtonHistory();

    // Return the button pressed
    return button_history[2].number;
  }

  return -1;
}

buttonHeld getButtonHeld() {

  uint8_t i;
  buttonHeld held;

  held.source = -1;
  held.target = -1;

  // Ensure all buttons are unprocessed
  for (i = 0; i < BUTTON_HISTORY_SIZE; i++) {
    if (button_history[i].processed) return held;
  }

  // Identify button down x2, button up x2
  if (button_history[0].pressed && button_history[1].pressed && !button_history[2].pressed && !button_history[3].pressed) {

    // Mark all as processed so we only process the button hold once
    for (i = 0; i < BUTTON_HISTORY_SIZE; i++) {
      button_history[i].processed = true;
    }

   // Serial.println("HELD");
   // printButtonHistory();


    held.source = button_history[0].number;
    held.target = button_history[1].number;
  }

  // TODO do we need to release this memory somehow?
  return held;
}


void printButtonHistory() {

  uint8_t i;

  for (i = 0; i < BUTTON_HISTORY_SIZE; i++) {
    Serial.printf("%d", button_history[i].number);
    Serial.printf(button_history[i].pressed ? "D" : "U");
    Serial.printf("%d ", button_history[i].processed);
  }

  Serial.println();
}
