struct buttonEvent {
  uint8_t number;
  bool pressed;
  bool processed;
};

// Because of use of modifier keys, we need to maintain the last two button key down and key up events
#define BUTTON_HISTORY_SIZE 4

static buttonEvent button_history[BUTTON_HISTORY_SIZE];
void setButtonState(uint8_t number, bool pressed);
int8_t getButtonPressed();
buttonHeld getButtonHeld();